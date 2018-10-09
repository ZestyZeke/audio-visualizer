//
// Created by zeke on 9/30/18.
//

#ifndef AUDIOVIS_ANALYZER_H
#define AUDIOVIS_ANALYZER_H


#include <fftw3.h>
#include <string>
#include <vector>
#include <aquila/global.h>

// hardcoded constants,
// adjusted as necessary
#define WINDOW_ALPHA 0.16
#define EWMA_ALPHA 0.3

class Analyzer {
public:
    Analyzer() = delete;
    Analyzer(const std::size_t fftSize);

    //@TODO: rename to analyze or something more generic.
    std::vector<double> applyFft(const std::vector<Aquila::SampleType> sampleBuffer);

private:
    void calcWindowVals();
    static double windowHanning(const std::size_t i, const std::size_t N);
    static double windowBlack(const std::size_t i, const std::size_t N);

    std::vector<double> squashBufferByFour(const std::vector<double> buffer);

    double *_in;
    fftw_complex *_out;
    fftw_plan _plan;
    std::size_t _FFT_SIZE;

    std::vector<double> _windowVals;

    static constexpr double PI {3.141592653589793};

    void applyEwma(std::vector<double>& currBuffer);

    void scaleLog(std::vector<double>& currBuffer);

    std::vector<double> _prevVals {};
};


#endif //AUDIOVIS_ANALYZER_H
