//
// Created by zeke on 9/30/18.
//

#ifndef AUDIOVIS_ENGINE_H
#define AUDIOVIS_ENGINE_H

#include <chrono>
#include <fstream>
#include <thread>
#include <atomic>

#include <aquila/source/WaveFile.h>

#include "Analyzer.h"
#include "Visualizer.h"
#include "Config.h"
#include "DataQueue.h"

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

    //@TODO: add description
    //@TODO: come up with better name than 'interva'l
    void copySamplesToBuffer(size_t fftBinIndex, std::vector<Aquila::SampleType>& sampleBuffer,
        Aquila::WaveFile& waveFile);

    ///
    /// \brief function that analyzer thread executes
    void threadRun();

    ///
    /// \brief starts the analyzer thread
    void startThread();

    ///
    /// \brief stops the analyzer thread
    void stopThread();

    ///
    /// \brief the total number of samples in the song
    size_t _NUM_SAMPLES;

    ///
    /// \brief the number of samples to grab at each iteration for fft processing
    const size_t _FFT_SIZE;

    //@TODO: give description
    const size_t _MAX_HEIGHT;

    ///
    /// \brief if processing occurred instantaneously, this would be the amount
    /// of time to wait inbetween iterations to ensure audio synchs with visuals
    std::chrono::milliseconds _FRAME_LENGTH;

    ///
    /// \brief first is left channel, second is right channel, handles to audio wav file
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

    ///
    /// \brief separate thread that performs work of analyzing samples
    std::thread _analyzerThread;

    ///
    /// \brief indicates to analyzer thread if it should continue
    std::atomic_bool _continueAnalyzing;

    ///
    /// \brief allows for synchronized data passing between this thread
    /// and analyzer thread
    DataQueue _dataQueue;
};


#endif //AUDIOVIS_ENGINE_H
