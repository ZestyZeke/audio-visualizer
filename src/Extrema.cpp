//
// Created by zeke on 1/30/19.
//

#include "Extrema.h"

void Extrema::update(const std::vector<double>& magnitudeList) {
    for (int i = 0; i < magnitudeList.size(); i++) {
        const double MAGNITUDE = magnitudeList[i];
        const double FREQ = i * _freqFactor;

        const int BIN_INDEX = findBin(FREQ);
        if (BIN_INDEX != -1 && MAGNITUDE > _peakMagnitudes[BIN_INDEX]) {
            _peakMagnitudes[BIN_INDEX] = MAGNITUDE;
        } if (MAGNITUDE > _absolutePeak) {
            _absolutePeak = MAGNITUDE;
        }
    }
}

int Extrema::findBin(const double freq) {
    for (int i = 0; i < _freqBin.size() - 1; i++) {
        const bool IN_BIN = (_freqBin[i] <= freq) && (freq <= _freqBin[i + 1]);
        if (IN_BIN) {
            return i;
        }
    }
    return -1;
}

void Extrema::setFrequencyBin(const std::vector<double> &freqBin) {
    _freqBin = freqBin;
    _peakMagnitudes = std::vector<double>(_freqBin.size() - 1, _NO_PEAK_FOUND);
}

void Extrema::simpleScale(std::vector<double>& magnitudeList) {
    for (double& val : magnitudeList) {
        val = (val / _absolutePeak) * 100;
    }
}

void Extrema::complexScale(std::vector<double>& magnitudeList) {
    for (int i = 0; i < magnitudeList.size(); i++) {
        const double FREQ = i * _freqFactor;
        double& magnitude = magnitudeList[i];

        const int BIN_INDEX = findBin(FREQ);
        if (BIN_INDEX != -1) {

            if (_peakMagnitudes[BIN_INDEX] == _NO_PEAK_FOUND) {
                magnitude = 0.0;
            } else {
                magnitude /= _peakMagnitudes[BIN_INDEX];
            }

        } else {
            magnitude = magnitude / _absolutePeak;
        }

        magnitude *= 100;
    }
}
