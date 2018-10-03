//
// Created by zeke on 8/30/18.
//

#ifndef AUDIOVIS_VISUALIZER_H
#define AUDIOVIS_VISUALIZER_H

#include <string>
#include <aquila/global.h>
#include <fftw3.h>


 //@TODO: idea for syncing - don't know how long FFT process takes. So start
 //@TODO: a timer, or record the time when process begins. After everything calculated,
 //@TODO: get time, and note difference. Sleep the extra. boom.

namespace Viz {
    void logOutput(std::ofstream& log, std::vector<double> buffer);

    template <class T>
    T getMax(std::vector<T> vec);

    template <class T>
    T getMin(std::vector<T> vec);
};

class Visualizer {
public:
    Visualizer();
    void displayToScreen(std::vector<double> buffer, const double MIN, const double MAX);
private:
    const std::size_t _NUM_ROWS = 100;

    std::vector<std::size_t> normalize(std::vector<double> buffer, const double MIN, const double MAX);
};


#endif //AUDIOVIS_VISUALIZER_H
