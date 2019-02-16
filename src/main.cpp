/*
 * audio-vis.cpp
 */
#include <iostream>
#include <SFML/Audio.hpp>
#include <unistd.h>
#include "Visualizer.h"
#include <fftw3.h>
#include <aquila/global.h>
#include <aquila/source/generator/SineGenerator.h>
#include <aquila/tools/TextPlot.h>
#include <aquila/source/WaveFile.h>
#include <aquila/transform/FftFactory.h>
#include "Engine.h"

int main(int argc, char **argv)
{
    // don't sync streams since we're not concerned with printing output
    std::ios::sync_with_stdio(false);

    std::string fileName {"444.wav"};
    if (argc == 2) {
        fileName = std::string(argv[1]);
    }

    Engine engine(fileName);
    engine.run();

    return 0;
}
