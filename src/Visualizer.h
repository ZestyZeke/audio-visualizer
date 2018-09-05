//
// Created by zeke on 8/30/18.
//

#ifndef AUDIOVIS_VISUALIZER_H
#define AUDIOVIS_VISUALIZER_H

#include <string>
#include <aquila/global.h>


/*
class Visualizer {

};
 */

namespace Viz {

    void displayToScreen(std::vector<Aquila::SampleType> sampleBuffer, const int MIN_SAMPLE,
    const int MAX_SAMPLE);

    void loadFile(std::string fileName);

    std::vector<std::size_t> normalize(std::vector<Aquila::SampleType> sampleBuffer, const double MIN_SAMPLE,
    const double MAX_SAMPLE);

    //static const std::size_t FFT_SIZE = 1024;
    static const std::size_t FFT_SIZE = 28;

    static const std::size_t NUM_ROWS = 100;

};


#endif //AUDIOVIS_VISUALIZER_H
