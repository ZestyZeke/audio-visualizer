//
// Created by zeke on 9/30/18.
//

#ifndef AUDIOVIS_ENGINE_H
#define AUDIOVIS_ENGINE_H

#include <string>
#include <aquila/source/WaveFile.h>
#include <chrono>
#include <fstream>
#include "Analyzer.h"
#include "Visualizer.h"

// hardcoded constants, adjusted as necessary
#define NUM_DISPLAY_BINS 500
#define FFT_SIZE 1024

class Engine {
public:
    Engine(const std::string fileName);
    void run();
private:
    void loop();
    std::chrono::milliseconds balanceTime(const std::chrono::milliseconds currDebt,
            const std::chrono::milliseconds timeElapsed);
    std::size_t _NUM_SAMPLES;
    std::size_t _FFT_SIZE;
    std::chrono::milliseconds _DELAY;
    Aquila::WaveFile _wav;
    Analyzer _analyzer;
    Visualizer _visualizer;
    std::ofstream _log;
};


#endif //AUDIOVIS_ENGINE_H
