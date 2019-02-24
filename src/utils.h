//
// Created by zeke on 2/16/19.
//

#ifndef AUDIOVIS_UTILS_H
#define AUDIOVIS_UTILS_H

#include <vector>
#include <algorithm>
#include <future>

namespace utils {

    ///
    /// \brief finds the frequency bin that the freq fits into
    /// \param freq the frequency to use
    /// \param freqBinList the list representing each boundary of each bin
    /// \return the 0-based index if successful, else -1
    int findBin(double freq, const std::vector<double>& freqBinList);

    ///
    /// \brief the implementation of the parallelized transform operation
    /// \tparam RandomIt each iterator should be Random Access
    /// \param begin1 the start point for the left container
    /// \param end1 the end point for the left container
    /// \param begin2 the start point for the right container
    /// \param end2 the end point for the right container
    template <typename RandomIt>
    void parallelTransform(RandomIt begin1, RandomIt end1, RandomIt begin2, RandomIt end2) {
            auto len = end1 - begin1;

            // base case
            if (len < 1000) {
                std::transform(begin1, end1, begin2, begin1,
                               std::plus<>());
                return;
            }

            RandomIt mid1 = begin1 + len / 2;
            RandomIt mid2 = begin2 + len / 2;

            // perform transform recursively on [mid:end] slice
            auto future = std::async(std::launch::async,
                                     parallelTransform<RandomIt>, mid1, end1, mid2, end2);

            // perform transform recursively on [begin:mid] slice
            parallelTransform(begin1, mid1, begin2, end2);
            future.wait();
    }

    ///
    /// \brief parallelized transform operation
    /// \tparam Container should satisfy RandomIterator
    /// \param vectorLeft the vector to store the result in
    /// \param vectorRight used with other vector to perform transform
    template <typename Container>
    void parallelTransform(Container& vectorLeft, Container& vectorRight) {
        parallelTransform(vectorLeft.begin(), vectorLeft.end(),
                          vectorRight.begin(), vectorRight.end());
    }

    ///
    /// \brief scales each value of a buffer, by
    /// 10 * log( el^2 / ref^2 ) / log(10)
    /// \param currBuffer the buffer to modify
    void scaleLog(std::vector<double>& currBuffer);

    ///
    /// \brief internal representation of PI
    constexpr double PI {3.141592653589793};

    ///
    /// \brief alpha constant to use for windowing functions
    //constexpr double WINDOW_ALPHA = 0.16;
    constexpr double WINDOW_ALPHA = 0.60;

    ///
    /// \brief applies the Hanning window function
    /// \param i the ith index
    /// \param N the size of the list
    /// \return windowed value
    double windowHanning(std::size_t i, std::size_t N);

    ///
    /// \brief applies the Black window function
    /// \param i the ith index
    /// \param N the size of the list
    /// \return windowed value
    double windowBlack(std::size_t i, std::size_t N);

    //@TODO: update documentation for any added parameters
    ///
    /// \brief generates the freq bin list
    /// \param sampleRate the samplingRate of the audio file
    /// \return a vector of frequencies representing the x-axis of the spectrum
    std::vector<double> generateFrequencyAxis(double sampleRate);


    ///
    /// \brief used to squash a given buffer by a factor,
    /// by averaging each  n elements
    /// \param buffer the buffer to squash
    /// \param squashFactor the factor to squash by
    /// \throws std::runtime_error if buffer is not exactly squashable by four
    /// \return a squashed buffer
    std::vector<double> squashBuffer(std::vector<double> buffer, std::size_t squashFactor);

};


#endif //AUDIOVIS_UTILS_H
