//
// Created by zeke on 9/30/18.
//

#ifndef AUDIOVIS_ENGINE_H
#define AUDIOVIS_ENGINE_H

#include <string>
#include <aquila/source/WaveFile.h>
#include <chrono>
#include <fstream>
#include "Analyzer.h"
#include "Visualizer.h"

// hardcoded constants, adjusted as necessary
//#define MAX_HEIGHT 500
#define MAX_HEIGHT 300 
//#define FFT_SIZE 1024
#define FFT_SIZE 2048

///
/// \class Engine
///
/// \brief runs high-level loop, and manages Visualizer and Analyzer
class Engine {
public:

    ///
    /// \brief Default Constructor not allowed
    Engine() = delete;

    ///
    /// \brief Constructor
    /// \param fileName
    Engine(const std::string fileName);

    ///
    /// \brief high level entry point for running the engine
    void run();

private:

    ///
    /// \brief performs the 'loop' of iteratively grabbing samples from song
    /// and all high-level processing that occurs with the grabbed samples
    void loop();

    ///
    /// \brief keeps track of 'debt' - how far behind visuals are from audio,
    /// and adjusts accordingly
    /// \param currDebt
    /// \param timeElapsed
    /// \return
    std::chrono::milliseconds balanceTime(const std::chrono::milliseconds currDebt,
            const std::chrono::milliseconds timeElapsed);

    ///
    /// \brief the total number of samples in the song
    std::size_t _NUM_SAMPLES;

    ///
    /// \brief the number of samples to grab at each iteration for fft processing
    std::size_t _FFT_SIZE;

    ///
    /// \brief if processing occured instantaneously, this would be the amount
    /// of time to wait inbetween iterations to ensure audio synchs with visuals
    std::chrono::milliseconds _DELAY;

    ///
    /// \brief a handle to the audio file in Wav format
    Aquila::WaveFile _wav;

    ///
    /// \brief a handle to the analyzer
    Analyzer _analyzer;

    ///
    /// \brief a handle to the visualizer
    Visualizer _visualizer;

    ///
    /// \brief log for debugging
    std::ofstream _log;

    Aquila::SampleType _max;
    Aquila::SampleType _min;
};


#endif //AUDIOVIS_ENGINE_H
