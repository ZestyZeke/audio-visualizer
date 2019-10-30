//
// Created by zeke on 9/30/18.
//

#ifndef AUDIOVIS_ENGINE_H
#define AUDIOVIS_ENGINE_H

#include <string_view>
#include <aquila/source/WaveFile.h>
#include <chrono>
#include <fstream>
#include "Analyzer.h"
#include "Visualizer.h"
#include "Config.h"

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
    Engine(const std::string& fileName, Config config);

    ///
    /// \brief high level entry point for running the engine
    void run();

private:

    ///
    /// \brief passes through all the samples to perform any pre-processing
    /// such as determining peak magnitudes
    void setup();

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

    //@TODO: add description
    //@TODO: come up with better name than 'interva'l
    void copySamplesToBuffer(std::size_t fftBinIndex, std::vector<Aquila::SampleType>& sampleBuffer,
        Aquila::WaveFile& waveFile);

    ///
    /// \brief the total number of samples in the song
    std::size_t _NUM_SAMPLES;

    ///
    /// \brief the number of samples to grab at each iteration for fft processing
    const std::size_t _FFT_SIZE;

    //@TODO: give description
    const std::size_t _MAX_HEIGHT;

    ///
    /// \brief if processing occured instantaneously, this would be the amount
    /// of time to wait inbetween iterations to ensure audio synchs with visuals
    std::chrono::milliseconds _DELAY;

    //@TODO: update...
    /*
    ///
    /// \brief a handle to the audio file in Wav format
    Aquila::WaveFile _wav;
     */
    // first is left, second is right
    std::pair<Aquila::WaveFile, Aquila::WaveFile> _wavFilePair;

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
