//
// Created by zeke on 2/24/19.
//

#ifndef AUDIOVIS_CONFIG_H
#define AUDIOVIS_CONFIG_H

#include <cstddef>
#include <string>

///
/// \struct Resolution
///
/// \brief simple struct that holds window resolution
struct Resolution {
    ///
    /// \brief the resolution in x pixels
    int x;

    ///
    /// \brief the resolution in y pixels
    int y;
};

///
/// \struct Config
///
/// \brief simple struct for holding configuration data
struct Config {

    ///
    /// \brief the window resolution
    Resolution resolution;

    ///
    /// \brief the size of the FFT to perform
    size_t fftSize;

    ///
    /// \brief the
    size_t maxHeight;

    ///
    /// \brief the number of vertical bins to divide the window into
    size_t numRows;

    ///
    /// \brief the title to display on the window
    std::string windowTitle;

    ///
    /// \brief used for adjusting the weight in the EWMA calculation
    double ewmaAlpha;

    ///
    /// \brief used for determining if simple scaling or complex scaling
    bool useSimpleScale;

    ///
    /// \brief function that grabs config info from a given file
    /// \param fileName the name of the file to be grapped
    void loadFile(std::string fileName);
};


#endif //AUDIOVIS_CONFIG_H
