//
// Created by zeke on 9/30/18.
//

#ifndef AUDIOVIS_ENGINE_H
#define AUDIOVIS_ENGINE_H

#include <string>
#include <aquila/source/WaveFile.h>
#include "Analyzer.h"

class Engine {
public:
    Engine(const std::string fileName);
    void run();
private:
    void loop();
    std::size_t _NUM_SAMPLES;
    std::size_t _FFT_SIZE;
    std::size_t _DELAY;
    Aquila::WaveFile _wav;
    Analyzer _analyzer;
};


#endif //AUDIOVIS_ENGINE_H
