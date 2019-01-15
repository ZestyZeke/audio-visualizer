//
// Created by zeke on 8/30/18.
//

#ifndef AUDIOVIS_VISUALIZER_H
#define AUDIOVIS_VISUALIZER_H

#include <string>
#include <aquila/global.h>
#include <fftw3.h>
#include <SFML/Graphics.hpp>

// hardcoded constants,
// adjusted as necessary
#define NUM_ROWS 100
#define RESOLUTION_X 800
#define RESOLUTION_Y 600
#define WINDOW_TITLE "Audio Visualizer"

//@TODO: get rid of?
namespace Viz {
    void logOutput(std::ofstream& log, std::vector<double> buffer);

    template <class T>
    T getMax(std::vector<T> vec);

    template <class T>
    T getMin(std::vector<T> vec);
};

class Visualizer {
public:
    Visualizer() :_color(sf::Color::Cyan),
    _window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), WINDOW_TITLE) { }

    void displayToScreen(std::vector<double> buffer, const double minHeight, const double maxHeight);

    inline bool isWindowOpen() { return _window.isOpen(); }

private:
    // transforms a vector of bins vs power
    // to bins vs 'ith row'
    // kind of like a stepped height
    std::vector<std::size_t> normalize(std::vector<double> buffer, const double minHeight, const double maxHeight);

    void checkBufferSize(const std::size_t size);

    void checkEvent();

    const std::size_t _NUM_ROWS = NUM_ROWS;

    sf::RenderWindow _window;

    const sf::Color _color;

    std::vector<sf::VertexArray> _rectangleList;
};


#endif //AUDIOVIS_VISUALIZER_H
