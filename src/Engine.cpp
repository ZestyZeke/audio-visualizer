//
// Created by zeke on 9/30/18.
//

#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>

#include <range/v3/all.hpp>

#include <SFML/Audio/Music.hpp>

#include <aquila/source/WaveFile.h>

#include "Engine.h"
#include "utils.h"

Engine::Engine(const std::string& fileName, Config config)
:_FFT_SIZE {config.fftSize},
_MAX_HEIGHT {config.maxHeight},
_wavFilePair {std::make_pair(Aquila::WaveFile(fileName, Aquila::StereoChannel::LEFT),
        Aquila::WaveFile(fileName, Aquila::StereoChannel::RIGHT))},
_analyzer {config, _wavFilePair.first.getSampleFrequency()},
_visualizer {config},
_log{"log.txt"},
_continueAnalyzing{true}
{
    // extract useful info from wav
    _NUM_SAMPLES = _wavFilePair.first.getSamplesCount();
    const size_t AUDIO_LENGTH = _wavFilePair.first.getAudioLength();
    const double delay = std::floor((AUDIO_LENGTH * _FFT_SIZE) / _NUM_SAMPLES);
    _FRAME_LENGTH = std::chrono::milliseconds(static_cast<int>(delay));
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
    startThread();

    song.play();

    loop();
    stopThread();
}

void Engine::copySamplesToBuffer(size_t fftBinIndex, std::vector<Aquila::SampleType> &sampleBuffer,
                                 Aquila::WaveFile &waveFile) {
    const size_t FFT_BIN_BEGIN = fftBinIndex * _FFT_SIZE;
    const size_t FFT_BIN_END = (fftBinIndex + 1) * _FFT_SIZE;

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

    for (size_t i = 0;
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

    auto logBeforeBreak = [] (size_t frameIndex, size_t numFrames) {
        std::cerr << "wait took too long on frameIndex " << frameIndex <<
        " with numFrames " << numFrames << std::endl;
    };

    const size_t NUM_FRAMES = std::floor(_NUM_SAMPLES / _FFT_SIZE);
    const auto START = system_clock::now();
    for (size_t i = 0; i < NUM_FRAMES && _visualizer.isWindowOpen(); i++) {

        std::optional<std::vector<double>> displayableData = _dataQueue.dequeue();
        if (!displayableData) {
            logBeforeBreak(i, NUM_FRAMES);
            break;
        }

        // sleep until at half frame time point
        std::this_thread::sleep_until(START + (i + .5) * _FRAME_LENGTH);
        _visualizer.displayToScreen(*displayableData, 0, _MAX_HEIGHT);

        displayableData = _dataQueue.dequeue();
        if (!displayableData) {
            logBeforeBreak(i, NUM_FRAMES);
            break;
        }

        // sleep until at end frame time point
        std::this_thread::sleep_until(START + (i + 1) * _FRAME_LENGTH);
        _visualizer.displayToScreen(*displayableData, 0, _MAX_HEIGHT);

    }
}

void Engine::threadRun() {

    const size_t NUM_FRAMES = std::floor(_NUM_SAMPLES / _FFT_SIZE);
    std::vector<Aquila::SampleType> sampleBufferLeft;
    std::vector<Aquila::SampleType> sampleBufferRight;
    std::vector<double> lastResult;
    for (size_t i = 0; i < NUM_FRAMES && _continueAnalyzing; i++) {

        copySamplesToBuffer(i, sampleBufferLeft, _wavFilePair.first);
        copySamplesToBuffer(i, sampleBufferRight, _wavFilePair.second);

        std::vector<double> displayableData = _analyzer.transform(sampleBufferLeft,
            sampleBufferRight);

        if (!lastResult.empty()) {
            // interpolate between last Result and current
            utils::interpolate(lastResult, displayableData);
            // enqueue this interpolation
            _dataQueue.enqueue(lastResult);
        }

        lastResult = displayableData;
        _dataQueue.enqueue(displayableData);

        // if end of loop, add an interpolation between last enqueued
        // and all 0's...
        if (i == NUM_FRAMES - 1) {
            const std::vector<double> zeroes (displayableData.size(), 0);
            utils::interpolate(displayableData, zeroes);
            _dataQueue.enqueue(displayableData);
        }
    }
}

void Engine::startThread() {
    _analyzerThread = std::thread(&Engine::threadRun, this);
}

void Engine::stopThread() {
    _continueAnalyzing = false;
    _analyzerThread.join();
}
