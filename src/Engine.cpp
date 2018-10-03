//
// Created by zeke on 9/30/18.
//

#include <SFML/Audio/Music.hpp>
#include "Engine.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <aquila/source/WaveFile.h>

Engine::Engine(const std::string fileName)
:_FFT_SIZE {1024}, _wav {fileName}, _analyzer {_FFT_SIZE}
{
    // extract useful info from wav
    //_wav = Aquila::WaveFile(fileName);
    _NUM_SAMPLES = _wav.getSamplesCount();
    const std::size_t AUDIO_LENGTH = _wav.getAudioLength();
    _DELAY = (AUDIO_LENGTH * _FFT_SIZE) / _NUM_SAMPLES;

    //_analyzer = Analyzer(_FFT_SIZE);
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
}

void Engine::loop() {
    std::vector<Aquila::SampleType> sampleBuffer;
    for (std::size_t i = 0; i < std::floor(_NUM_SAMPLES / _FFT_SIZE); i++) {

        sampleBuffer.resize(_FFT_SIZE, 0);
        auto it = sampleBuffer.begin();
        for (std::size_t j = i * _FFT_SIZE; j < (i + 1) * _FFT_SIZE; j++)
            *it++ = _wav.sample(j);

        std::vector<double> fftResult = _analyzer.applyFft(sampleBuffer);

        //@TODO: display to screen stuff.
    }
}
