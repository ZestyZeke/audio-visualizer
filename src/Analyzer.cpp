//
// Created by zeke on 9/30/18.
//

#include <cmath>
#include "Analyzer.h"

Analyzer::Analyzer(const std::size_t fftSize) {
    _FFT_SIZE = fftSize;
    _in = static_cast<double *>(fftw_malloc(sizeof(double) * _FFT_SIZE));
    _out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * _FFT_SIZE));
    _plan = fftw_plan_dft_r2c_1d(_FFT_SIZE, _in, _out, FFTW_MEASURE);

    calcWindowVals();
}

void Analyzer::calcWindowVals() {
    auto windowFunc = windowHanning;
    _windowVals = std::vector<double> (_FFT_SIZE, 0);
    for (std::size_t i = 0; i < _FFT_SIZE; i++) {
        _windowVals[i] = windowFunc(i, _FFT_SIZE);
    }
}

double Analyzer::windowBlack(const std::size_t i, const std::size_t N) {
    constexpr double ALPHA = .16; // hardcoded
    constexpr double TERM1 = (1 - ALPHA) / 2;
    const double     TERM2 = 0.5 * cos((2 * PI * i) / (N - 1));
    const double     TERM3 = 0.5 * ALPHA * cos((4 * PI * i) / (N - 1));
    return TERM1 - TERM2 + TERM3;
}

double Analyzer::windowHanning(const std::size_t i, const std::size_t N) {
    return 0.5 * (1 - cos((2 * PI * i) / (N - 1)));
}

std::vector<double> Analyzer::applyFft(const std::vector<Aquila::SampleType> sampleBuffer) {
    // setup
    const std::size_t N = sampleBuffer.size();

    // copy dynamic vals
    for (std::size_t i = 0; i < N; i++) {
        _in[i]  = sampleBuffer[i];
        _in[i] *= _windowVals[i];
    }

    // execute
    fftw_execute(_plan);

    // only half of the values mean anything.
    // get values and pass them back.
    // don't care about complex values.
    std::vector<double> power (N / 2, 0);
    for (std::size_t i = 0; i < N / 2; i++) {
        power[i] = std::abs(_out[i][0]);
    }

    // squash so that displayable on screen.
    return squashBufferByFour(power);
}

//@TODO: make more... generic to say the least.
std::vector<double> Analyzer::squashBufferByFour(const std::vector<double> buffer) {
    if (buffer.size() % 4 != 0)
        throw std::runtime_error("buffer is not squashable by four");
    std::vector<double> squashedBuffer (buffer.size() / 4, 0);
    auto it = squashedBuffer.begin();
    for (std::size_t i = 0; i < buffer.size(); i += 4) {
        double avg = buffer[i] + buffer[i + 1] + buffer[i + 2] + buffer[i + 3];
        avg /= 4.0;
        *it++ = avg;
    }
    return squashedBuffer;
}