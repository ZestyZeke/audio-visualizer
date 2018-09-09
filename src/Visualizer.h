//
// Created by zeke on 8/30/18.
//

#ifndef AUDIOVIS_VISUALIZER_H
#define AUDIOVIS_VISUALIZER_H

#include <string>
#include <aquila/global.h>
#include <fftw3.h>


/*
class Visualizer {

};
 */
 //@TODO: idea for syncing - don't know how long FFT process takes. So start
 //@TODO: a timer, or record the time when process begins. After everything calculated,
 //@TODO: get time, and note difference. Sleep the extra. boom.

namespace Viz {

    void displayToScreen(std::vector<Aquila::SampleType> sampleBuffer, const int MIN_SAMPLE,
    const int MAX_SAMPLE);

    void loadFile(std::string fileName);

    std::vector<std::size_t> normalize(std::vector<Aquila::SampleType> sampleBuffer, const double MIN_SAMPLE,
    const double MAX_SAMPLE);

    static const std::size_t FFT_SIZE = 1024;
    //static const std::size_t FFT_SIZE = 28;
    //static const std::size_t FFT_SIZE = 256;
    //static const std::size_t FFT_SIZE = 200;

    static const std::size_t NUM_ROWS = 100;
    static std::size_t DELAY;
    //@TODO: need to downmix the channels somehow...
    std::vector<double> applyFft(std::vector<Aquila::SampleType> sampleBuffer, double* in,
    fftw_complex *out, fftw_plan plan);

    template <class T>
    T getMax(std::vector<T> vec);

    template <class T>
    T getMin(std::vector<T> vec);

    static std::vector<double> windowVals;
    double windowHanning(const std::size_t i, const std::size_t N);
    double windowBlack(const std::size_t i, const std::size_t N);
    void calcWindowVals(const std::size_t N);

    static const double PI = 3.141592653589793;

    std::vector<double> squashBufferByFour(const std::vector<double> buffer);
};


#endif //AUDIOVIS_VISUALIZER_H
