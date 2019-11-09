//
// Created by zeke on 9/30/18.
//

#ifndef AUDIOVIS_ANALYZER_H
#define AUDIOVIS_ANALYZER_H


#include <fftw3.h>
#include <string>
#include <vector>
#include <aquila/global.h>
#include "Extrema.h"
#include "Config.h"

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
    Analyzer(Config config, double samplingRate);

    //@TODO: update description
    ///
    /// \brief top level function for applying a fast fourier transform on a buffer of samples,
    /// amongst other transformations such that the data will be easily displayable
    /// \param sampleBuffer the samples to transform
    /// \return the result of all the transformations
    std::vector<double> transform(const std::vector<Aquila::SampleType>& sampleBufferLeft,
        const std::vector<Aquila::SampleType>& sampleBufferRight);

    //@TODO: update description
    ///
    /// \brief top level function for applying a fast fourier transform on a buffer of samples,
    /// and subsequently storing the extrema for use on a later pass-through
    /// \param sampleBuffer the samples to transform
    void updateExtrema(const std::vector<Aquila::SampleType>& sampleBufferLeft,
        const std::vector<Aquila::SampleType>& sampleBufferRight);

private:

    ///
    /// \brief function for applying a fast fourier transform on a buffer of samples
    /// \param sampleBuffer the buffer of samples to apply the fast fourier transform on
    /// \return a vector of doubles representing frequency vs power
    std::vector<double> applyFft(const std::vector<Aquila::SampleType>& sampleBuffer);


    ///
    /// \brief puts fft results into a spectrum that is log-scaled w respect to frequency
    /// \param buffer the buffer to squash into frequency bins for display
    /// \return a vector of the samples transformed into a spectrum
    std::vector<double> spectrumize(const std::vector<double> buffer);

    //@TODO: add documentation
    void setupFftPlan();

    ///
    /// \brief precomputes the _windowVals array to use for windowing,
    /// according to a given window function.
    void calcWindowVals();

    ///
    /// \brief applies a Exponential Weighted Moving Average to the buffer values
    /// \param currBuffer the buffer to apply EWMA against
    void applyEwma(std::vector<double>& currBuffer);

private:

    ///
    /// \brief a convenience class for holding the extrema of the fft data
    Extrema _extrema;

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
    const std::size_t _FFT_SIZE;

    ///
    /// \brief the pre-computed values to use for windowing
    std::vector<double> _windowVals;

    ///
    /// \brief stores buffer values from previous pass-through. Used to
    /// perform EWMA
    std::vector<double> _prevVals {};

    ///
    /// \brief these values represent the sides of each frequency bin
    /// will be used to place fftResults into a frequency bin for visualizing
    std::vector<double> _freqBinList;

    ///
    /// \brief the sample rate of the song
    /// used for frequency calculations
    const double _SAMPLE_RATE;

    ///
    /// \brief the ewma alpha factor for smoothing
    const double _EWMA_ALPHA;

    ///
    /// \brief true for simple scaling, false for complex scaling
    const bool _USE_SIMPLE_SCALE;

};


#endif //AUDIOVIS_ANALYZER_H
