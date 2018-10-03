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
:_FFT_SIZE {1024}, _wav {fileName}, _analyzer {_FFT_SIZE}, _log{"log.txt"}
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
    std::vector<Aquila::SampleType> sampleBuffer;
    for (std::size_t i = 0; i < std::floor(_NUM_SAMPLES / _FFT_SIZE); i++) {
        const auto THEN = system_clock::now();

        sampleBuffer.resize(_FFT_SIZE, 0);
        auto it = sampleBuffer.begin();
        for (std::size_t j = i * _FFT_SIZE; j < (i + 1) * _FFT_SIZE; j++)
            *it++ = _wav.sample(j);

        std::vector<double> fftResult = _analyzer.applyFft(sampleBuffer);

        //@TODO: display to screen stuff.
        // the getMin, getMax stuff needs to be replaced with a more absolute solution.
        //displayToScreen(fftResult, getMin(fftResult), getMax(fftResult);
        _visualizer.displayToScreen(fftResult, 0, 2056); // @TODO: this is hardcoded
        //@TODO: use actual vals, rather than 0 and 100

        const auto NOW = system_clock::now();
        const milliseconds TIME_ELAPSED = duration_cast<milliseconds>(NOW - THEN);

        if (TIME_ELAPSED < _DELAY) {
            std::this_thread::sleep_for(_DELAY - TIME_ELAPSED);
        } else {
            _log << "TOOK TOO LONG by this amount (milliseconds): " << (TIME_ELAPSED - _DELAY).count() << std::endl;
        }
    }
}
