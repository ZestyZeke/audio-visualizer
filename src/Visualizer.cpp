//
// Created by zeke on 8/30/18.
//

#include "Visualizer.h"
#include <aquila/global.h>
#include <aquila/source/WaveFile.h>
#include <aquila/transform/FftFactory.h>
#include <aquila/tools/TextPlot.h>
#include <vector>
#include <aquila/source/generator/SineGenerator.h>
#include <iostream>
#include <algorithm>

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
                normalizedBuffer.push_back(i);
            }
        }
    }
    return normalizedBuffer;
}


void Viz::displayToScreen(std::vector<Aquila::SampleType> sampleBuffer, const int MIN_SAMPLE,
    const int MAX_SAMPLE) {
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
    Aquila::WaveFile wav ("444.wav");
    const std::size_t NUM_SAMPLES = wav.getSamplesCount();

    /*
    std::vector<Aquila::SampleType> sampleBuffer {};
    for (std::size_t i = FFT_SIZE; i < 2*FFT_SIZE; i++) {
        sampleBuffer.push_back(wav.sample(i));
        std::cout << wav.sample(i) << std::endl;
    }

    // record max and min sample values
    const Aquila::SampleType MIN_SAMPLE = *std::min_element(sampleBuffer.begin(), sampleBuffer.end());
    const Aquila::SampleType MAX_SAMPLE = *std::max_element(sampleBuffer.begin(), sampleBuffer.end());
    // display it somehow
    displayToScreen(sampleBuffer, MIN_SAMPLE, MAX_SAMPLE);
     */
    //const Aquila::SampleType MAX_SAMPLE = *std::max_element(sampleBuffer.begin(), sampleBuffer.end());
    Aquila::SampleType MIN_SAMPLE = wav.sample(0);
    Aquila::SampleType MAX_SAMPLE = wav.sample(0);
    for (auto sample : wav) {
        if (sample < MIN_SAMPLE)
            MIN_SAMPLE = sample;
        if (sample > MAX_SAMPLE)
            MAX_SAMPLE = sample;
    }

    for (std::size_t i = 0; i < std::floor(NUM_SAMPLES / FFT_SIZE); i += FFT_SIZE) {
        std::vector<Aquila::SampleType> sampleBuffer {};
        for (std::size_t j = i * FFT_SIZE; j < (i + 1) * FFT_SIZE; j++) {
            sampleBuffer.push_back(wav.sample(j));
        }
        displayToScreen(sampleBuffer, MIN_SAMPLE, MAX_SAMPLE);
    }

    /*
     * Apparently can't use fft with Aquila? Low key so mad and frustrated with just trying to find and get
     * a library to work.
    Aquila::SpectrumType spectrum = fft->fft(sampleBuffer.data());
    Aquila::TextPlot plt ("signal");
    plt.plotSpectrum(spectrum);
     */
}
