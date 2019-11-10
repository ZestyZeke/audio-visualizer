//
// Created by zeke on 2/16/19.
//

#include "utils.h"
#include <algorithm>
#include <functional>
#include <future>
#include <cmath>
#include <numeric>
#include <range/v3/all.hpp>

int utils::findBin(const double freq, const std::vector<double>& freqBinList) {
    auto fitsInBin = [freq]
        (const double freqBinLeft, const double freqBinRight) -> bool {
        return (freqBinLeft <= freq) && (freq <= freqBinRight);
    };

    // overloads std::adjacent_find
    auto it = ranges::v3::adjacent_find(freqBinList, fitsInBin);
    if (it == freqBinList.end()) {
        return -1;
    } else {
        return std::distance(freqBinList.begin(), it);
    }
}

void utils::scaleLog(std::vector<double> &currBuffer) {
    constexpr double REF = 1.0;
    constexpr double BASE = 10.0;

    constexpr double TERM_DENOM = REF * REF; // why can't std::pow() be constexpr?
    const double FACTOR = 10 / std::log(BASE); // why can't std::log() be constexpr?

    auto scaleFunc = [=] (double x) {
        if (x == 0) {
            return x; // applying std::log(x) will return -inf
        }

        const double TERM = (x * x) / TERM_DENOM;
        return FACTOR * std::log(TERM);
    };

    using ranges::v3::action::transform;
    currBuffer |= transform(scaleFunc);
}

double utils::windowBlack(const size_t i, const size_t N) {
    constexpr double ALPHA = WINDOW_ALPHA;
    constexpr double TERM1 = (1 - ALPHA) / 2;
    const double     TERM2 = 0.5 * cos((2 * PI * i) / (N - 1));
    const double     TERM3 = 0.5 * ALPHA * cos((4 * PI * i) / (N - 1));
    return TERM1 - TERM2 + TERM3;
}

double utils::windowHanning(const size_t i, const size_t N) {
    return 0.5 * (1 - cos((2 * PI * i) / (N - 1)));
}

std::vector<double> utils::generateFrequencyAxis(const double sampleRate) {
    // generate a frequency axis that's log-scaled from 0
    // to SAMPLE_RATE / 2

    //@TODO: this still doesn't give enough bins on the lower frequency side...
    // (x1, y1) = (1, 10)
    // (x2, y2) = (50, SAMPLE_RATE / 2)

    const int x1 = 1;
    const double y1 = 10;
    const int x2 = 200;
    const double y2 = sampleRate / 2;

    // y = a * e ^ (b * x)
    // b = ln(y1 / y2) / (x1 - x2)
    // a = y1 / e ^ (b * x1)

    const double b = log(y1 / y2) / (x1 - x2);
    const double a = y1 / exp(b * x1);
    auto func = [a, b] (const int x) -> double { return a * exp(b * x); };

    using namespace ranges;
    return view::ints(x1, x2 + 1) | view::transform(func);
}

std::vector<double> utils::squashBuffer(std::vector<double> buffer, size_t squashFactor) {
    if (buffer.size() % squashFactor != 0) {
        throw std::runtime_error("buffer is not squashable by " + std::to_string(squashFactor));
    }

    std::vector<double> squashedBuffer (buffer.size() / 4, 0);

    auto it = squashedBuffer.begin();
    auto begin = buffer.begin();

    for (size_t i = 0; i < buffer.size(); i += squashFactor) {
        auto pos = begin + i;
        auto end = pos + squashFactor;
        const double AVG = std::accumulate(pos, end, 0) / squashFactor;
        *it++ = AVG;
    }

    return squashedBuffer;
}

void utils::interpolate(std::vector<double> &startData,
    const std::vector<double> &endData) {
    for (auto i = 0; i < startData.size(); i++) {
        startData[i] = (startData[i] + endData[i]) / 2;
    }
}