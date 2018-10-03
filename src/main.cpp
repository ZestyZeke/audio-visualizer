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
    const std::string FILE_NAME = "444.wav";
    Engine engine(FILE_NAME);
    engine.run();

    return 0;
}
