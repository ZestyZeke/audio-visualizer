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

class Engine {
public:
    Engine(const std::string fileName);
    void run();
private:
    void loop();
    std::size_t _NUM_SAMPLES;
    std::size_t _FFT_SIZE;
    std::chrono::milliseconds _DELAY;
    Aquila::WaveFile _wav;
    Analyzer _analyzer;
    Visualizer _visualizer;
    std::ofstream _log;
};


#endif //AUDIOVIS_ENGINE_H
