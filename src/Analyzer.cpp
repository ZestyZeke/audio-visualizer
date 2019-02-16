//
// Created by zeke on 9/30/18.
//

#include <cmath>
#include <iostream>
#include <array>
#include "Analyzer.h"
#include <limits>
#include <algorithm>
#include <numeric>
#include <future>
#include "utils.h"

Analyzer::Analyzer(const std::size_t fftSize, const double samplingRate) {
    _FFT_SIZE = fftSize;
    _in = static_cast<double *>(fftw_malloc(sizeof(double) * _FFT_SIZE));
    _out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * _FFT_SIZE));

    _sampleRate = samplingRate;

    setupFftPlan();

    // set up containers that can be pre-calculated
    calcWindowVals();
    _freqBinList = utils::generateFrequencyAxis(samplingRate);
    _extrema.setFrequencyBin(_freqBinList);
    _extrema.setFrequencyFactor(_sampleRate / _FFT_SIZE);
}

void Analyzer::setupFftPlan() {
    const std::string wisdomFileName {"wisdon.config"};
    const int LOADED =
        fftw_import_wisdom_from_filename(wisdomFileName.c_str());

    _plan = fftw_plan_dft_r2c_1d(static_cast<int>(_FFT_SIZE),
        _in, _out, FFTW_EXHAUSTIVE);

    if (!LOADED) {
        fftw_export_wisdom_to_filename(wisdomFileName.c_str());
    }
}

void Analyzer::calcWindowVals() {
    auto windowFunc = utils::windowHanning;
    _windowVals = std::vector<double> (_FFT_SIZE, 0);
    for (std::size_t i = 0; i < _FFT_SIZE; i++) {
        _windowVals[i] = windowFunc(i, _FFT_SIZE);
    }
}

//@TODO: have two versions of everything... for double channel and mono channel
std::vector<double> Analyzer::transform(const std::vector<Aquila::SampleType>& sampleBufferLeft,
    const std::vector<Aquila::SampleType>& sampleBufferRight) {

    // apply Fast Fourier Transform
    /*
    //@TODO: perform these in separate threads?
    //@TODO: could 'this' context be troublesome...?
    //@TODO: for these two to work, in / out need to be managed separately...

     //@TODO: to work... can modify applyFFT to take 'in' and 'out' as args...
     //@TODO: and also need to take 'plan' separately...
     //@TODO: just kind of need to have multiple definitions for this case
    auto promiseLeft = std::async(std::launch::async,
        &Analyzer::applyFft, this,
        sampleBufferLeft);
    auto promiseRight = std::async(std::launch::async,
        &Analyzer::applyFft, this,
        sampleBufferRight);

    std::vector<double> power = promiseLeft.get();
    std::vector<double> powerRight = promiseRight.get();
     */

    std::vector<double> power = applyFft(sampleBufferLeft);
    std::vector<double> powerRight = applyFft(sampleBufferRight);

    //@TODO: perform these in separate threads?
    // combine the two channels
    utils::parallelTransform<std::vector<double>>(power, powerRight);

    // scale to be a percentage of the absolute peak
    //_extrema.simpleScale(power);
    _extrema.complexScale(power);

    auto spectrum = spectrumize(power);

    // commenting this out will help bring the 'voice' out
    utils::scaleLog(spectrum);

    // commenting this out will make the display feel 'jittery'
    applyEwma(spectrum);

    return spectrum;
}

void Analyzer::updateExtrema(const std::vector<Aquila::SampleType>& sampleBufferLeft,
    const std::vector<Aquila::SampleType>& sampleBufferRight) {

    //@TODO: run these in separate threads?
    // apply Fast Fourier Transform
    std::vector<double> power = applyFft(sampleBufferLeft);

    std::vector<double> powerRight = applyFft(sampleBufferRight);

    // combine the two channels
    utils::parallelTransform<std::vector<double>>(power, powerRight);

    // check for extrema
    _extrema.update(power);

}

std::vector<double> Analyzer::applyFft(const std::vector<Aquila::SampleType>& sampleBuffer) {
    // setup
    const std::size_t N = sampleBuffer.size();

    // copy dynamic vals
    for (std::size_t i = 0; i < N; i++) {
        _in[i]  = sampleBuffer[i];
        _in[i] *= _windowVals[i]; // apply windowing function
    }

    // execute
    fftw_execute(_plan);

    // only half of the values mean anything.
    // get values and pass them back.
    // don't care about complex values.
    std::vector<double> power (N / 2, 0);
    for (std::size_t i = 0; i < N / 2; i++) {
        power[i] = std::abs(_out[i][0]);
    }

    return power;
}

void Analyzer::applyEwma(std::vector<double> &currBuffer) {

    // check if first iteration
    if (_prevVals.size() == 0) {
        _prevVals = currBuffer;
        return;
    }

    constexpr double ALPHA = EWMA_ALPHA;
    for (int i = 0; i < currBuffer.size() && i < _prevVals.size(); i++) {
        // apply exponential weighted moving average
        currBuffer[i] = ALPHA * _prevVals[i] + (1 - ALPHA) * currBuffer[i];
        // update prevVals for next time this function is called
        _prevVals[i] = currBuffer[i];
    }
}

std::vector<double> Analyzer::squashBufferByFour(const std::vector<double> buffer) {
    if (buffer.size() % 4 != 0)
        throw std::runtime_error("buffer is not squashable by four");
    std::vector<double> squashedBuffer (buffer.size() / 4, 0);
    auto it = squashedBuffer.begin();
    for (std::size_t i = 0; i < buffer.size(); i += 4) {
        double avg = buffer[i] + buffer[i + 1] + buffer[i + 2] + buffer[i + 3];
        avg /= 4.0;
        *it++ = avg;
    }
    return squashedBuffer;
}

std::vector<double> Analyzer::spectrumize(const std::vector<double> magnitudeList) {

    std::vector<double> peakMagnitudes (_freqBinList.size() - 1, 0);

    for (int i = 0; i < magnitudeList.size(); i++) {
        const double MAGNITUDE = magnitudeList[i];
        const double FREQ = i * _sampleRate / _FFT_SIZE;

        const int BIN_INDEX = utils::findBin(FREQ, _freqBinList);
        if (BIN_INDEX != -1 && MAGNITUDE > peakMagnitudes[BIN_INDEX]) {
            peakMagnitudes[BIN_INDEX] = MAGNITUDE;
        }
    }

    return peakMagnitudes;
}

