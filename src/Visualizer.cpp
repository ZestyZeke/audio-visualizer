//
// Created by zeke on 8/30/18.
//

#include "Visualizer.h"
#include <vector>

Visualizer::Visualizer(Config config)
:_NUM_ROWS(config.numRows),
_color(sf::Color::Cyan),
_window(sf::VideoMode(config.resolution.x, config.resolution.y), config.windowTitle)
{

}

std::vector<size_t> Visualizer::normalize(std::vector<double> buffer, const double minHeight,
        const double maxHeight) {
    std::vector<size_t> normalizedBuffer;
    // overloads assignment operator as push_back() member function
    auto backInserter = std::back_inserter(normalizedBuffer);

    const double step = (maxHeight - minHeight) / static_cast<double>(_NUM_ROWS);
    // here, i represents the ith row to be included in
    for (auto sample : buffer) {

        bool inInterval {false};
        for (size_t i = 0; i < _NUM_ROWS; i++) {
            inInterval = minHeight + i * step <= sample &&
                    sample <= minHeight + (i + 1) * step;
            if (inInterval) {
                backInserter = i;
                break;
            }
        }

        if (inInterval)
            continue;

        if (sample < minHeight) {
            backInserter = static_cast<size_t>(minHeight + 1);
        } else if (sample > maxHeight) {
            backInserter = static_cast<size_t>(maxHeight - 1);
        }
    }
    return normalizedBuffer;
}

void Visualizer::displayToScreen(std::vector<double> buffer, const double minHeight, const double maxHeight) {
    checkEvent();
    if (!_window.isOpen()) {
        return;
    }

    std::vector<size_t> normalizedBuffer = normalize(buffer, minHeight, maxHeight);

    checkBufferSize(normalizedBuffer.size());

    _window.clear(sf::Color::Black);

    const double ROW_HEIGHT = _window.getSize().y / _NUM_ROWS;

    for (int i = 0; i < normalizedBuffer.size(); i++) {
        const size_t numRows = normalizedBuffer[i];
        sf::VertexArray& rectangle = _rectangleList[i];

        const float TOP = static_cast<float>(_window.getSize().y - numRows * ROW_HEIGHT);
        // only update top vertices
        rectangle[0].position.y = TOP;
        rectangle[1].position.y = TOP;

        _window.draw(rectangle);
    }

    _window.display();
}

void Visualizer::checkBufferSize(const size_t size) {
    if (_rectangleList.size() == size) {
        return;
    }


    // need to redo all the rectangles' coordinates and colors
    _rectangleList.resize(size, sf::VertexArray(sf::Quads, 4));

    constexpr float START  = 0.f;
    const float WIDTH  = _window.getSize().x / _rectangleList.size();

    for (int i = 0; i < size; i++) {
        sf::VertexArray& rectangle = _rectangleList[i];

        // update vertices
        const float LEFT = START + WIDTH * i;
        const float RIGHT = START + WIDTH * (i + 1);
        const float BOTTOM = _window.getSize().y;
        const float TOP = BOTTOM; // this will get updated when actually processing normalized buffer

        rectangle[0].position = sf::Vector2f(LEFT, TOP);
        rectangle[1].position = sf::Vector2f(RIGHT, TOP);
        rectangle[2].position = sf::Vector2f(RIGHT, BOTTOM);
        rectangle[3].position = sf::Vector2f(LEFT, BOTTOM);

        // set colors
        for (int j = 0; j < 4; j++) {
            rectangle[j].color = _color;
        }

    }
}

void Visualizer::checkEvent() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();

        } else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Space) {
                updateColors();
            } else if (event.key.code == sf::Keyboard::Escape) {
                _window.close();
            }
        }
    }
}

void Visualizer::updateColors() {

    // update color preference
    if (_color == sf::Color::Cyan) {
        _color = sf::Color::Green;
    } else if (_color == sf::Color::Green) {
        _color = sf::Color::Magenta;
    } else if (_color == sf::Color::Magenta) {
        _color = sf::Color::Cyan;
    }

    // update the colors in the _rectangleList
    for (sf::VertexArray& rectangle : _rectangleList) {
        for (int i = 0; i < 4; i++) {
            rectangle[i].color = _color;
        }
    }
}
