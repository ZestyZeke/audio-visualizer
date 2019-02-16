//
// Created by zeke on 2/16/19.
//

#include "utils.h"
#include <algorithm>
#include <functional>
#include <future>
#include <cmath>
#include <numeric>

int utils::findBin(const double freq, const std::vector<double>& freqBinList) {
    auto fitsInBin = [freq]
        (const double freqBinLeft, const double freqBinRight) -> bool {
        return (freqBinLeft <= freq) && (freq <= freqBinRight);
    };

    auto it = std::adjacent_find(freqBinList.begin(), freqBinList.end(), fitsInBin);
    if (it == freqBinList.end()) {
        return -1;
    } else {
        return std::distance(freqBinList.begin(), it);
    }
}

void utils::scaleLog(std::vector<double> &currBuffer) {
    constexpr double REF = 1.0;
    constexpr double BASE = 10.0;
    auto scaleFunc = [REF, BASE] (double& x) {
        const double TERM = std::pow(x, 2) / std::pow(REF, 2);
        x = 10 * std::log(TERM) / std::log(BASE);
    };

    std::for_each(currBuffer.begin(), currBuffer.end(), scaleFunc);
}

double utils::windowBlack(const std::size_t i, const std::size_t N) {
    constexpr double ALPHA = WINDOW_ALPHA;
    constexpr double TERM1 = (1 - ALPHA) / 2;
    const double     TERM2 = 0.5 * cos((2 * PI * i) / (N - 1));
    const double     TERM3 = 0.5 * ALPHA * cos((4 * PI * i) / (N - 1));
    return TERM1 - TERM2 + TERM3;
}

double utils::windowHanning(const std::size_t i, const std::size_t N) {
    return 0.5 * (1 - cos((2 * PI * i) / (N - 1)));
}

std::vector<double> utils::generateFrequencyAxis(const double sampleRate) {
    // generate a frequency axis that's log-scaled from 0
    // to SAMPLE_RATE / 2

    //@TODO: this still doesn't give enough bins on the lower frequency side...
    // (x1, y1) = (1, 10)
    // (x2, y2) = (50, SAMPLE_RATE / 2)

    const double x1 = 1;
    const double y1 = 10;
    const double x2 = 200;
    const double y2 = sampleRate / 2;

    // y = a * e ^ (b * x)
    // b = ln(y1 / y2) / (x1 - x2)
    // a = y1 / e ^ (b * x1)

    const double b = log(y1 / y2) / (x1 - x2);
    const double a = y1 / exp(b * x1);
    auto func = [a, b] (const double x) -> double { return a * exp(b * x); };

    std::vector<double> x_axis_vals;
    for (int i = x1; i <= x2; i++) {
        x_axis_vals.push_back(func(i));
    }
    return x_axis_vals;
}

std::vector<double> utils::squashBuffer(std::vector<double> buffer, std::size_t squashFactor) {
    if (buffer.size() % squashFactor != 0) {
        throw std::runtime_error("buffer is not squashable by " + std::to_string(squashFactor));
    }

    std::vector<double> squashedBuffer (buffer.size() / 4, 0);

    auto it = squashedBuffer.begin();
    auto begin = buffer.begin();

    for (std::size_t i = 0; i < buffer.size(); i += squashFactor) {
        auto pos = begin + i;
        auto end = pos + squashFactor;
        const double AVG = std::accumulate(pos, end, 0) / squashFactor;
        *it++ = AVG;
    }

    return squashedBuffer;
}