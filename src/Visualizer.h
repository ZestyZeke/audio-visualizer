//
// Created by zeke on 8/30/18.
//

#ifndef AUDIOVIS_VISUALIZER_H
#define AUDIOVIS_VISUALIZER_H

#include <string>
#include <aquila/global.h>
#include <fftw3.h>
#include <SFML/Graphics.hpp>
#include "Config.h"

///
/// \class Visualizer
///
/// \brief Responsible for taking buffers and displaying them in an SFML window
class Visualizer {
public:

    ///
    /// \brief Constructor
    /// \param config to use for Populating member values
    Visualizer(Config config);

    ///
    /// \brief Destructor
    ~Visualizer() = default;

    ///
    /// \brief public access point for taking fft transformed samples and displaying on screen
    /// \param buffer buffer to use for frequency vs power information
    /// \param minHeight relative to bottom of window
    /// \param maxHeight relative to bottom of window
    void displayToScreen(std::vector<double> buffer, const double minHeight, const double maxHeight);

    ///
    /// \returns true if window is currently open
    inline bool isWindowOpen() { return _window.isOpen(); }

private:
    ///
    /// \brief transforms a vector of frequency bins vs power into frequency bins vs vertical resolution bins
    /// \param buffer vector to transform
    /// \param minHeight relative to bottom of window
    /// \param maxHeight relative to bottom of window
    /// \return normalized buffer
    std::vector<size_t> normalize(std::vector<double> buffer, const double minHeight, const double maxHeight);

    ///
    /// \brief compares the buffer's size to that of _rectangleList and reconstructs the list if necessary
    /// \param size the to-be-transformed buffer's size
    void checkBufferSize(const size_t size);

    ///
    /// \brief checks to see if an event has occured
    void checkEvent();

    ///
    /// \brief for when the color to display has changed
    void updateColors();

    ///
    /// \brief number of vertical bins to divide the window into
    const size_t _NUM_ROWS;

    ///
    /// \brief the color to use for each rectangle
    sf::Color _color;

    ///
    /// \brief the window to use for rendering shapes
    sf::RenderWindow _window;

    ///
    /// \brief the list of rectangles to be drawn onto the window
    std::vector<sf::VertexArray> _rectangleList;
};


#endif //AUDIOVIS_VISUALIZER_H
