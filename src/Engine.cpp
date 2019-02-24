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
#include <range/v3/all.hpp>

Engine::Engine(const std::string fileName)
:_FFT_SIZE {FFT_SIZE},
_wavFilePair {std::make_pair(Aquila::WaveFile(fileName, Aquila::StereoChannel::LEFT),
        Aquila::WaveFile(fileName, Aquila::StereoChannel::RIGHT))},
_analyzer {_FFT_SIZE, _wavFilePair.first.getSampleFrequency()},
_log{"log.txt"}
{
    // extract useful info from wav
    _NUM_SAMPLES = _wavFilePair.first.getSamplesCount();
    const std::size_t AUDIO_LENGTH = _wavFilePair.first.getAudioLength();
    const double delay = std::floor((AUDIO_LENGTH * _FFT_SIZE) / _NUM_SAMPLES);
    _DELAY = std::chrono::milliseconds(static_cast<int>(delay));
}

void Engine::run() {

    // prepare to play music
    sf::Music song;
    if (!song.openFromFile(_wavFilePair.first.getFilename())
        || !song.openFromFile(_wavFilePair.second.getFilename())) {
        std::cerr << "Wasn't able to open file" << std::endl;
        return;
    }
    setup();

    song.play();

    loop();
}

void Engine::copySamplesToBuffer(std::size_t fftBinIndex, std::vector<Aquila::SampleType> &sampleBuffer,
                                 Aquila::WaveFile &waveFile) {
    const std::size_t FFT_BIN_BEGIN = fftBinIndex * _FFT_SIZE;
    const std::size_t FFT_BIN_END = (fftBinIndex + 1) * _FFT_SIZE;

    using namespace ranges;
    auto toSample = [&] (int i) -> Aquila::SampleType {
        return waveFile.sample(i);
    };
    sampleBuffer = view::ints(FFT_BIN_BEGIN, FFT_BIN_END) |
        view::transform(toSample);
}

void Engine::setup() {
    std::vector<Aquila::SampleType> sampleBufferLeft;
    std::vector<Aquila::SampleType> sampleBufferRight;

    for (std::size_t i = 0;
         i < std::floor(_NUM_SAMPLES / _FFT_SIZE);
         i++) {

        copySamplesToBuffer(i, sampleBufferLeft, _wavFilePair.first);
        copySamplesToBuffer(i, sampleBufferRight, _wavFilePair.second);

        _analyzer.updateExtrema(sampleBufferLeft, sampleBufferRight);
    }
}

void Engine::loop() {
    using std::chrono::system_clock;
    using std::chrono::milliseconds;
    using std::chrono::duration_cast;

    milliseconds debt {0};
    std::vector<Aquila::SampleType> sampleBufferLeft;
    std::vector<Aquila::SampleType> sampleBufferRight;

    for (std::size_t i = 0;
         i < std::floor(_NUM_SAMPLES / _FFT_SIZE) && _visualizer.isWindowOpen();
         i++) {

        const auto START = system_clock::now();

        copySamplesToBuffer(i, sampleBufferLeft, _wavFilePair.first);
        copySamplesToBuffer(i, sampleBufferRight, _wavFilePair.second);

        //@TODO: where i left off...
        std::vector<double> displayableData = _analyzer.transform(sampleBufferLeft,
            sampleBufferRight);

        constexpr int MAX = MAX_HEIGHT;
        _visualizer.displayToScreen(displayableData, 0, MAX);

        const auto END = system_clock::now();
        const milliseconds TIME_ELAPSED = duration_cast<milliseconds>(END - START);

        debt = balanceTime(debt, TIME_ELAPSED);
    }
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
