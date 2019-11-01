/*
 * audio-vis.cpp
 */
#include <iostream>
#include <unistd.h>
#include <experimental/filesystem>

#include <SFML/Audio.hpp>

#include <fftw3.h>

#include <aquila/global.h>
#include <aquila/source/generator/SineGenerator.h>
#include <aquila/tools/TextPlot.h>
#include <aquila/source/WaveFile.h>
#include <aquila/transform/FftFactory.h>

#include "Visualizer.h"
#include "Engine.h"
#include "Config.h"

int main(int argc, char **argv) {
    // don't sync streams since we're not concerned with printing output
    std::ios::sync_with_stdio(false);

    Config config;
    config.loadFile("config.xml");

    std::string fileName {"444.wav"};
    if (argc == 2) {
        fileName = std::string(argv[1]);
    }

    // verify that file exists
    if (std::experimental::filesystem::exists(fileName)) {
        Engine engine(fileName, config);
        engine.run();
    } else {
        std::cerr << "file [" << fileName <<
        "] does not exist in current directory" << std::endl;
    }
}
