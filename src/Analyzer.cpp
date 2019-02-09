//
// Created by zeke on 9/30/18.
//

#include <cmath>
#include <iostream>
#include <array>
#include "Analyzer.h"
#include <limits>

double windowFunctions::windowBlack(const std::size_t i, const std::size_t N) {
    constexpr double ALPHA = WINDOW_ALPHA;
    constexpr double TERM1 = (1 - ALPHA) / 2;
    const double     TERM2 = 0.5 * cos((2 * PI * i) / (N - 1));
    const double     TERM3 = 0.5 * ALPHA * cos((4 * PI * i) / (N - 1));
    return TERM1 - TERM2 + TERM3;
}

double windowFunctions::windowHanning(const std::size_t i, const std::size_t N) {
    return 0.5 * (1 - cos((2 * PI * i) / (N - 1)));
}

Analyzer::Analyzer(const std::size_t fftSize, const double samplingRate) {
    _FFT_SIZE = fftSize;
    _in = static_cast<double *>(fftw_malloc(sizeof(double) * _FFT_SIZE));
    _out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * _FFT_SIZE));
    _plan = fftw_plan_dft_r2c_1d(static_cast<int>(_FFT_SIZE),
            _in, _out, FFTW_MEASURE);

    _sampleRate = samplingRate;

    // set up containers that can be pre-calculated
    calcWindowVals();
    _freqBin = generateFrequencyAxis();
    _extrema.setFrequencyBin(_freqBin);
    _extrema.setFrequencyFactor(_sampleRate / _FFT_SIZE);
}

void Analyzer::calcWindowVals() {
    auto windowFunc = windowFunctions::windowHanning;
    _windowVals = std::vector<double> (_FFT_SIZE, 0);
    for (std::size_t i = 0; i < _FFT_SIZE; i++) {
        _windowVals[i] = windowFunc(i, _FFT_SIZE);
    }
}

std::vector<double> Analyzer::transform(const std::vector<Aquila::SampleType>& sampleBuffer) {
    // apply Fast Fourier Transform
    std::vector<double> power = applyFft(sampleBuffer);

    // scale to be a percentage of the absolute peak
    //_extrema.simpleScale(power);
    _extrema.complexScale(power);

    auto spectrum = spectrumize(power);

    scaleLog(spectrum);

    applyEwma(spectrum);

    return spectrum;
}

void Analyzer::updateExtrema(const std::vector<Aquila::SampleType> &sampleBuffer) {
    // apply Fast Fourier Transform
    std::vector<double> power = applyFft(sampleBuffer);

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

void Analyzer::scaleLog(std::vector<double> &currBuffer) {
    constexpr double REF = 1.0;
    constexpr double BASE = 10.0;
    for (double& el : currBuffer) {
        const double TERM = (el * el) / (REF * REF);
        el = 10 * std::log(TERM) / std::log(BASE);
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

int Analyzer::findBin(const double freq) {
    for (int i = 0; i < _freqBin.size() - 1; i++) {
        const bool IN_BIN = (_freqBin[i] <= freq) && (freq <= _freqBin[i + 1]);
        if (IN_BIN) {
            return i;
        }
    }
    return -1;
}

std::vector<double> Analyzer::spectrumize(const std::vector<double> magnitudeList) {

    std::vector<double> peakMagnitudes (_freqBin.size() - 1, 0);

    for (int i = 0; i < magnitudeList.size(); i++) {
        const double MAGNITUDE = magnitudeList[i];
        const double FREQ = i * _sampleRate / _FFT_SIZE;

        const int BIN_INDEX = findBin(FREQ);
        if (BIN_INDEX != -1 && MAGNITUDE > peakMagnitudes[BIN_INDEX]) {
            peakMagnitudes[BIN_INDEX] = MAGNITUDE;
        }
    }

    return peakMagnitudes;
}

std::vector<double> Analyzer::generateFrequencyAxis() {
    // generate a frequency axis that's log-scaled from 0
    // to SAMPLE_RATE / 2

    // (x1, y1) = (1, 10)
    // (x2, y2) = (50, SAMPLE_RATE / 2)

    const double x1 = 1;
    const double y1 = 10;
    const double x2 = 200;
    const double y2 = _sampleRate / 2;

    // y = a * e ^ (b * x)
    // b = ln(y1 / y2) / (x1 - x2)
    // a = y1 / e ^ (b * x1)

    const double b = log(y1 / y2) / (x1 - x2);
    const double a = y1 / exp(b * x1);
    auto func = [a, b] (const double x) -> double { return a * exp(b * x); };

    std::vector<double> x_axis_vals;
    for (int i = x1; i <= x2; i++) {
        x_axis_vals.push_back(func(i));
    }
    return x_axis_vals;
}
