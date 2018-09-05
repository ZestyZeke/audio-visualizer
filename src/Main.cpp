/*
 * audio-vis.cpp
 */
#include <iostream>
#include <SFML/Audio.hpp>
#include <unistd.h>
#include "Analyzable.hpp"
#include "Visualizer.h"
#include <fftw3.h>
#include <aquila/global.h>
#include <aquila/source/generator/SineGenerator.h>
#include <aquila/tools/TextPlot.h>
#include <aquila/source/WaveFile.h>
#include <aquila/transform/FftFactory.h>

int main(int argc, char **argv)
{
    // calculate the FFT

    Viz::loadFile("444.wav");
    return 0;
}
