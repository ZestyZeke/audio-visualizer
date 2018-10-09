//
// Created by zeke on 9/30/18.
//

#include <SFML/Audio/Music.hpp>
#include "Engine.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <aquila/source/WaveFile.h>
#include <thread>

Engine::Engine(const std::string fileName)
:_FFT_SIZE {FFT_SIZE}, _wav {fileName}, _analyzer {_FFT_SIZE}, _log{"log.txt"}
{
    // extract useful info from wav
    _NUM_SAMPLES = _wav.getSamplesCount();
    const std::size_t AUDIO_LENGTH = _wav.getAudioLength();
    const double delay = std::floor((AUDIO_LENGTH * _FFT_SIZE) / _NUM_SAMPLES);
    _DELAY = std::chrono::milliseconds(static_cast<int>(delay));

}

void Engine::run() {

    // prepare to play music
    sf::Music song;
    if (!song.openFromFile(_wav.getFilename())) {
        std::cerr << "Wasn't able to open file" << std::endl;
        return;
    }
    song.play();
    const auto THEN = std::chrono::system_clock::now();

    loop();

    const auto NOW = std::chrono::system_clock::now();
    std::cout << "seconds passed: " << std::chrono::
    duration_cast<std::chrono::seconds>(NOW - THEN).count() << std::endl;
}

void Engine::loop() {
    using namespace std::chrono;
    milliseconds debt {0};
    std::vector<Aquila::SampleType> sampleBuffer;
    for (std::size_t i = 0; i < std::floor(_NUM_SAMPLES / _FFT_SIZE); i++) {
        const auto THEN = system_clock::now();

        sampleBuffer.resize(_FFT_SIZE, 0);
        auto it = sampleBuffer.begin();
        for (std::size_t j = i * _FFT_SIZE; j < (i + 1) * _FFT_SIZE; j++)
            *it++ = _wav.sample(j);

        std::vector<double> fftResult = _analyzer.applyFft(sampleBuffer);

        constexpr int MAX = NUM_DISPLAY_BINS;
        _visualizer.displayToScreen(fftResult, 0, MAX);

        const auto NOW = system_clock::now();
        const milliseconds TIME_ELAPSED = duration_cast<milliseconds>(NOW - THEN);

        debt = balanceTime(debt, TIME_ELAPSED);
    }
    std::cout << "final debt (ms): " << debt.count() << std::endl;
}

std::chrono::milliseconds Engine::balanceTime(const std::chrono::milliseconds currDebt,
        const std::chrono::milliseconds timeElapsed) {
    using namespace std::chrono;

    if (timeElapsed < _DELAY) {
        // have some time to spare after passing through analyzer and visualizer
        // can use this time to pay off debt, or to just sleep the remaining time.
        const milliseconds AVAILABLE_TIME = _DELAY - timeElapsed;

        // check if debt can be payed this cycle
        if (currDebt < AVAILABLE_TIME) {
            std::this_thread::sleep_for(AVAILABLE_TIME - currDebt);
            return 0ms;
        }

        // final case: currDebt > AVAILABLE_TIME
        // pay back debt by not sleeping, and update the currDebt
        return currDebt - AVAILABLE_TIME;

    } else {
        // don't have time to spare
        // adjust debt for falling behind
        const milliseconds OVERFLOW = timeElapsed - _DELAY;
        return currDebt + OVERFLOW;
    }
}
