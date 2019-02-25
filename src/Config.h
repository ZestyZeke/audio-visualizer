//
// Created by zeke on 2/24/19.
//

#ifndef AUDIOVIS_CONFIG_H
#define AUDIOVIS_CONFIG_H

#include <cstddef>
#include <string>

struct Resolution {
    int x;
    int y;
};

struct Config {
    Resolution resolution;
    std::size_t fftSize;
    std::size_t maxHeight;
    std::size_t numRows;
    std::string windowTitle;
    double ewmaAlpha;

    void loadFile(std::string fileName);
};


#endif //AUDIOVIS_CONFIG_H
