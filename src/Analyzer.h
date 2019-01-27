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
#define EWMA_ALPHA 0.3

namespace windowFunctions {

    ///
    /// \brief internal representation of PI
    constexpr double PI {3.141592653589793};

    ///
    /// \brief alpha constant to use for windowing functions
    //constexpr double WINDOW_ALPHA = 0.16;
    constexpr double WINDOW_ALPHA = 0.60;

    ///
    /// \brief applys the Hanning window function
    /// \param i
    /// \param N
    /// \return windowed value
    double windowHanning(const std::size_t i, const std::size_t N);

    ///
    /// \brief applys the Black window function
    /// \param i
    /// \param N
    /// \return windowed value
    double windowBlack(const std::size_t i, const std::size_t N);
};

///
/// \class Analyzer
///
/// \brief for performing the Fast Fourier Transform on samples
class Analyzer {
public:

    ///
    /// \brief Default Constructor not allowed
    Analyzer() = delete;

    ///
    /// \brief Constructor
    /// \param fftSize the size of the FastFourierTransform
    Analyzer(const std::size_t fftSize);

    ///
    /// \brief top level function for applying a fast fourier transform on a buffer of samples
    /// \param sampleBuffer the buffer of samples to apply the fast fourier transform on
    /// \return a vector of doubles representing frequency vs power
    std::vector<double> applyFft(const std::vector<Aquila::SampleType> sampleBuffer);

private:

    ///
    /// \brief precomputes the _windowVals array to use for windowing,
    /// according to a given window function.
    void calcWindowVals();

    ///
    /// \brief used to squash a given buffer by a factor of four,
    /// by averaging each four elements
    /// \param buffer the buffer to squash
    /// \throws std::runtime_error if buffer is not exactly squashable by four
    /// \return a squashed buffer
    std::vector<double> squashBufferByFour(const std::vector<double> buffer);

    ///
    /// \brief applies a Exponential Weighted Moving Average to the buffer values
    /// \param currBuffer the buffer to apply EWMA against
    void applyEwma(std::vector<double>& currBuffer);

    ///
    /// \brief scales each value of a buffer, by
    /// 10 * log(el ^2 / ref^2 ) / log(10)
    /// \param currBuffer
    void scaleLog(std::vector<double>& currBuffer);

    ///
    /// \brief pointer that references an array to use for passing the buffer
    /// to fft functions
    double *_in;

    ///
    /// \brief pointer that references an array to use for receiving transformed
    /// complex values from the fft functions
    fftw_complex *_out;

    ///
    /// \brief the fftw plan to use for performing the fft transformation
    fftw_plan _plan;

    ///
    /// \brief the size of the buffer passing into the fft transform
    std::size_t _FFT_SIZE;

    ///
    /// \brief the pre-computed values to use for windowing
    std::vector<double> _windowVals;

    ///
    /// \brief stores buffer values from previous pass-through. Used to
    /// perform EWMA
    std::vector<double> _prevVals {};
};


#endif //AUDIOVIS_ANALYZER_H
