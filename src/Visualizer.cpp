//
// Created by zeke on 8/30/18.
//

#include "Visualizer.h"
#include "Analyzable.hpp"
#include <aquila/global.h>
#include <aquila/source/WaveFile.h>
#include <aquila/transform/FftFactory.h>
#include <aquila/tools/TextPlot.h>
#include <vector>
#include <aquila/source/generator/SineGenerator.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fftw3.h>

std::vector<std::size_t> Viz::normalize(std::vector<Aquila::SampleType> sampleBuffer, const double MIN_SAMPLE,
                                               const double MAX_SAMPLE) {
    std::vector<std::size_t> normalizedBuffer;
    double step = static_cast<double>(MAX_SAMPLE - MIN_SAMPLE) / static_cast<double>(NUM_ROWS);
    // here, i represents the ith row to be included in
    for (auto sample : sampleBuffer) {
        for (std::size_t i = 0; i < NUM_ROWS; i++) {
            const bool IN_INTERVAL = MIN_SAMPLE + i * step <= sample &&
                    sample <= MIN_SAMPLE + (i + 1) * step;
            if (IN_INTERVAL) {
                normalizedBuffer.push_back(i++); // @TODO: kind of gross - clean up later
                break;
            }
        }
    }
    return normalizedBuffer;
}


void Viz::displayToScreen(std::vector<Aquila::SampleType> sampleBuffer, const int MIN_SAMPLE,
    //@TODO: replace with some OpenGL
    const int MAX_SAMPLE) {
    // wait a second, to give screen time to clear.
    //const auto DELAY = std::chrono::milliseconds(200);
    const auto delay = std::chrono::milliseconds(DELAY);
    std::this_thread::sleep_for(delay);

    // clear the screen lazy like
    for (int i = 0; i < 100; i++) std::cout << '\n';
    std::cout << std::endl;

    // now, loop and print.
    const auto SYMBOL = '@';
    const auto COLOR_BEGIN = "\033[1;36m";
    const auto COLOR_END = "\033[0m";

    std::vector<std::size_t> normalizedBuffer = normalize(sampleBuffer, MIN_SAMPLE, MAX_SAMPLE);
    const std::size_t BUFF_MAX = *std::max_element(normalizedBuffer.begin(), normalizedBuffer.end());

    for (int row = NUM_ROWS; row > 0; row--) {
        for (auto s : normalizedBuffer) {
            if (s >= row) {
                std::cout << COLOR_BEGIN << SYMBOL << COLOR_END;
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
}

void Viz::loadFile(std::string fileName) {
    std::cout << "hello" << std::endl;
    // input signal parameters
    Aquila::WaveFile wav (fileName);
    const std::size_t NUM_SAMPLES = wav.getSamplesCount();

    // milliseconds
    const std::size_t AUDIO_LENGTH = wav.getAudioLength();

    DELAY = (AUDIO_LENGTH * FFT_SIZE) / NUM_SAMPLES;
    //std::cout << "DELAY1 == " << DELAY << "milliseconds" << std::endl;
    //DELAY = (FFT_SIZE * wav.getBytesPerSec()) / wav.getBytesPerSample();
    const auto MILLISECONDS_PER_SECONDS = 1000;
    DELAY = (FFT_SIZE * wav.getBytesPerSample() * MILLISECONDS_PER_SECONDS) / wav.getBytesPerSec();
    std::cout << "DELAY == " << DELAY << " milliseconds" << std::endl;
    std::cout << "AUDIO_LENGTH == " << AUDIO_LENGTH << " milliseconds" << std::endl;
    std::cout << "FFT_SIZE == " << FFT_SIZE << " samples" << std::endl;
    std::cout << "NUM_SAMPLES == " << NUM_SAMPLES << " samples" << std::endl;
    std::cout << "CHANNELS == " << wav.getChannelsNum() << " channels" << std::endl;

    Aquila::SampleType MIN_SAMPLE = wav.sample(0);
    Aquila::SampleType MAX_SAMPLE = wav.sample(0);
    for (auto sample : wav) {
        if (sample < MIN_SAMPLE)
            MIN_SAMPLE = sample;
        if (sample > MAX_SAMPLE)
            MAX_SAMPLE = sample;
    }

    // prepare stuff for fft
    fftw_complex *in, *out;
    in = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * FFT_SIZE));
    out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * FFT_SIZE));
    fftw_plan plan = fftw_plan_dft_1d(FFT_SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // now begin playing music in background.
    sf::Analyzable song;
    if (!song.openFromFile(fileName))
        return;
    song.play();
    auto then = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < std::floor(NUM_SAMPLES / FFT_SIZE); i++) {
        std::vector<Aquila::SampleType> sampleBuffer{};
        for (std::size_t j = i * FFT_SIZE; j < (i + 1) * FFT_SIZE; j++) {
            sampleBuffer.push_back(wav.sample(j));
        }
        std::vector<double> fftResult = applyFft(sampleBuffer, in, out, plan);
        //@TODO: grab samples and do fft on them...
        //displayToScreen(sampleBuffer, MIN_SAMPLE, MAX_SAMPLE);
        displayToScreen(fftResult, getMin(fftResult), getMax(fftResult));
        //std::cout << "iter: " << i << std::endl;
    }
    auto now = std::chrono::system_clock::now();
    std::cout << "time passed: " << std::chrono::duration_cast<std::chrono::seconds>(now - then).count() << std::endl;

}

std::vector<double> Viz::applyFft(std::vector<Aquila::SampleType> sampleBuffer, fftw_complex *in, fftw_complex *out,
fftw_plan plan) {
    // setup
    const std::size_t N = sampleBuffer.size();
    /*
    fftw_complex *in, *out;
    in = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * N));
    out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * N));
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
     */

    // copy dynamic vals
    for (int i = 0; i < N; i++) {
        in[i][0] = sampleBuffer[i];
        in[i][1] = 0;
    }

    // execute
    fftw_execute(plan);

    // get values and pass them back.
    // don't care about complex values.
    std::vector<double> result (N, 0);
    for (int i = 0; i < N; i++) {
        result[i] = out[i][0];
        //std::cout << "result[" << i << "] == " << result[i] << std::endl;
    }
    return result;
}

template <class T>
T Viz::getMax(std::vector<T> vec) {
    T max = vec[0];
    for (auto v : vec) {
        if (v > max)
            max = v;
    }
    return max;
}

template <class T>
T Viz::getMin(std::vector<T> vec) {
    T min = vec[0];
    for (auto v : vec) {
        if (v < min)
            min = v;
    }
    return min;
}
