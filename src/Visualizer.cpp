//
// Created by zeke on 8/30/18.
//

#include "Visualizer.h"
#include <aquila/global.h>
#include <aquila/source/WaveFile.h>
#include <aquila/transform/FftFactory.h>
#include <aquila/tools/TextPlot.h>
#include <vector>
#include <aquila/source/generator/SineGenerator.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fftw3.h>
#include <cmath>
#include <fstream>
#include <SFML/Audio/Music.hpp>

std::vector<std::size_t> Visualizer::normalize(std::vector<double> buffer, const double MIN, const double MAX) {
    std::vector<std::size_t> normalizedBuffer;
    const double step = (MAX - MIN) / static_cast<double>(_NUM_ROWS);
    // here, i represents the ith row to be included in
    for (auto sample : buffer) {
        for (std::size_t i = 0; i < _NUM_ROWS; i++) {
            const bool IN_INTERVAL = MIN + i * step <= sample &&
                    sample <= MIN + (i + 1) * step;
            if (IN_INTERVAL) {
                normalizedBuffer.push_back(i); // @TODO: kind of gross - clean up later
                break;
            }
        }
    }
    return normalizedBuffer;
}

void Visualizer::displayToScreen(std::vector<double> buffer, const double MIN, const double MAX) {
    //@TODO: rethink min / max thing

    // clear the screen lazy like
    for (int i = 0; i < 100; i++) std::cout << '\n';
    std::cout << std::endl;


    // now, loop and print.
    //const auto SYMBOL = '@';
    static const auto SYMBOL = '.';
    static const auto COLOR_BEGIN = "\033[1;36m";
    static const auto COLOR_END = "\033[0m";

    std::vector<std::size_t> normalizedBuffer = normalize(buffer, MIN, MAX);
    const std::size_t BUFF_MAX = *std::max_element(normalizedBuffer.begin(), normalizedBuffer.end());

    for (int row = _NUM_ROWS; row > 0; row--) {
        for (auto s : normalizedBuffer) {
            if (s >= row) {
                std::cout << COLOR_BEGIN << SYMBOL << COLOR_END;
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
}
