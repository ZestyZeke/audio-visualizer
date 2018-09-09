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
#include <cmath>
#include <fstream>

void logOutput(std::ofstream& log, std::vector<double> buffer) {
    for (auto b : buffer) {
        log << b << ' ';
    }
    log << '\n';
}

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
    const int MAX_SAMPLE) {
    //@TODO: replace with some OpenGL
    // wait a second, to give screen time to clear.
    //const auto DELAY = std::chrono::milliseconds(200);
    const auto delay = std::chrono::milliseconds(DELAY);
    std::this_thread::sleep_for(delay);

    // clear the screen lazy like
    for (int i = 0; i < 100; i++) std::cout << '\n';
    std::cout << std::endl;

    // now, loop and print.
    //const auto SYMBOL = '@';
    static const auto SYMBOL = '.';
    static const auto COLOR_BEGIN = "\033[1;36m";
    static const auto COLOR_END = "\033[0m";

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
    double *in;
    fftw_complex *out;
    in = static_cast<double *>(fftw_malloc(sizeof(double) * FFT_SIZE));
    out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * FFT_SIZE));
    fftw_plan plan = fftw_plan_dft_r2c_1d(FFT_SIZE, in, out, FFTW_ESTIMATE);

    // pre calculate window function values
    calcWindowVals(FFT_SIZE);

    // now begin playing music in background.
    sf::Analyzable song;
    if (!song.openFromFile(fileName))
        return;
    song.play();
    auto then = std::chrono::system_clock::now();
    std::ofstream log ("log.txt");
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
        //logOutput(log, fftResult);
    }
    auto now = std::chrono::system_clock::now();
    std::cout << "time passed: " << std::chrono::duration_cast<std::chrono::seconds>(now - then).count() << std::endl;

}

//@TODO: make these into a precalculated array.
double Viz::windowHanning(const std::size_t i, const std::size_t N) {
    //@NOTE: apply before fft
    return 0.5 * (1 - cos((2 * PI * i) / (N - 1)));
}

double Viz::windowBlack(const std::size_t i, const std::size_t N) {
    static const double ALPHA = .16;
    const double TERM1 = (1 - ALPHA) / 2;
    const double TERM2 = 0.5 * cos((2 * PI * i) / (N - 1));
    const double TERM3 = 0.5 * ALPHA * cos((4 * PI * i) / (N - 1));
    return TERM1 - TERM2 + TERM3;
}

void Viz::calcWindowVals(const std::size_t N) {
    auto windowFunc = windowHanning;
    windowVals = std::vector<double> (N, 0);
    for (std::size_t i = 0; i < N; i++) {
        windowVals[i] = windowFunc(i, N);
    }
}

std::vector<double> Viz::applyFft(std::vector<Aquila::SampleType> sampleBuffer, double *in, fftw_complex *out,
fftw_plan plan) {
    // setup
    const std::size_t N = sampleBuffer.size();

    // copy dynamic vals
    for (int i = 0; i < N; i++) {
        in[i] = sampleBuffer[i];
        in[i] *= windowVals[i];
        //in[i][0] *= windowBlack(i, N);
    }

    // execute
    fftw_execute(plan);

    //@NOTE: only half of the values mean anything.
    // get values and pass them back.
    // don't care about complex values.
    std::vector<double> power (N / 2, 0);
    for (int i = 0; i < N / 2; i++) {
        power[i] = std::abs(out[i][0]);
        //power[i] = out[i][0] * out[i][0] + out[i][1] * out[i][1];
                       //@NOTE: have no clue why can't access out[i][1]
        //std::cout << "result[" << i << "] == " << result[i] << std::endl;
    }
    return squashBufferByFour(power);
}

std::vector<double> Viz::squashBufferByFour(const std::vector<double> buffer) {
    if (buffer.size() % 4 != 0)
        throw std::runtime_error("blah");
    std::vector<double> squashedBuffer;
    for (int i = 0; i < buffer.size(); i += 4) {
        double avg = buffer[i] + buffer[i + 1] + buffer[i + 2] + buffer[i + 3];
        avg /= 4.0;
        squashedBuffer.push_back(avg);
    }
    return squashedBuffer;
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
