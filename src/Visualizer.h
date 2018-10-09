//
// Created by zeke on 8/30/18.
//

#ifndef AUDIOVIS_VISUALIZER_H
#define AUDIOVIS_VISUALIZER_H

#include <string>
#include <aquila/global.h>
#include <fftw3.h>

// hardcoded constants,
// adjusted as necessary
#define NUM_ROWS 100

namespace Viz {
    void logOutput(std::ofstream& log, std::vector<double> buffer);

    template <class T>
    T getMax(std::vector<T> vec);

    template <class T>
    T getMin(std::vector<T> vec);
};

class Visualizer {
public:
    Visualizer() = default;
    void displayToScreen(std::vector<double> buffer, const double MIN, const double MAX);
private:
    const std::size_t _NUM_ROWS = NUM_ROWS;

    std::vector<std::size_t> normalize(std::vector<double> buffer, const double MIN, const double MAX);
};


#endif //AUDIOVIS_VISUALIZER_H
