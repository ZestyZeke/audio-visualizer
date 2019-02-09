//
// Created by zeke on 1/30/19.
//

#ifndef AUDIOVIS_EXTREMA_H
#define AUDIOVIS_EXTREMA_H

#include <vector>
#include <limits>

///
/// \class Extrema
///
/// \brief holds the extrema from analyzing the fft spectrum of all of the samples
class Extrema {
public:

    ///
    /// \brief Constructor
    Extrema() = default;

    ///
    /// \brief updates private members with values from the magnitudeList
    /// \param magnitudeList the magnitudes, from applying the fft transformation
    void update(const std::vector<double>& magnitudeList);

    ///
    /// \brief copies frequency bin from owning class and defaults _peakMagnitudes member
    /// \param freqBin the frequency bin to copy vals from
    void setFrequencyBin(const std::vector<double>& freqBin);

    ///
    /// \brief sets member to match owning class
    /// \param freqFactor to use for finding the frequency of a given index
    inline void setFrequencyFactor(const double freqFactor) { _freqFactor = freqFactor; }

    ///
    /// \brief modifies the magnitudesList s.t. each val is a percentage of the absolute peak
    /// \param magnitudeList the list to modify
    void simpleScale(std::vector<double>& magnitudeList);

    ///
    /// \brief modifies the magnitudesList s.t. each val is a percentage of the peak
    /// in the matching frequency bin
    /// \param magnitudeList the list to modify
    void complexScale(std::vector<double>& magnitudeList);

    ///
    /// \brief returns the absolute peak value
    /// \return absolute peak
    inline double getMax() { return _absolutePeak; }

private:

    //@TODO: this exact function is defined in Analyzer
    ///
    /// \brief finds the frequency bin that the freq fits into
    /// \param freq the frequency to use
    /// \return the 0-based index if successful, else -1
    int findBin(const double freq);

    ///
    /// \brief the frequency bins. Each value is a 'boundary' and 'bins' are defined in-between
    std::vector<double> _freqBin;

    ///
    /// \brief the largest magnitudes in each frequency bin
    std::vector<double> _peakMagnitudes;

    ///
    /// \brief the absolute largest magnitude across all frequency bins
    double _absolutePeak = _NO_PEAK_FOUND;

    ///
    /// \brief the scalar to multiply index by to get frequency
    double _freqFactor;

    ///
    /// \brief default value to use, when showing that no peak was found for a given frequency
    /// bin
    static constexpr double _NO_PEAK_FOUND = std::numeric_limits<double>::lowest();

};


#endif //AUDIOVIS_EXTREMA_H
