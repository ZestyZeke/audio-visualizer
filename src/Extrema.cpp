//
// Created by zeke on 1/30/19.
//

#include "Extrema.h"
#include "utils.h"

void Extrema::update(const std::vector<double>& magnitudeList) {
    for (int i = 0; i < magnitudeList.size(); i++) {
        const double MAGNITUDE = magnitudeList[i];
        const double FREQ = i * _freqFactor;

        const int BIN_INDEX = utils::findBin(FREQ, _freqBinList);
        if (BIN_INDEX != -1 && MAGNITUDE > _peakMagnitudeList[BIN_INDEX]) {
            _peakMagnitudeList[BIN_INDEX] = MAGNITUDE;
        } if (MAGNITUDE > _absolutePeak) {
            _absolutePeak = MAGNITUDE;
        }
    }
}

void Extrema::setFrequencyBin(const std::vector<double> &freqBin) {
    _freqBinList = freqBin;
    _peakMagnitudeList = std::vector<double>(_freqBinList.size() - 1, _NO_PEAK_FOUND);
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

        const int BIN_INDEX = utils::findBin(FREQ, _freqBinList);
        if (BIN_INDEX != -1) {

            if (_peakMagnitudeList[BIN_INDEX] == _NO_PEAK_FOUND) {
                magnitude = 0.0;
            } else {
                magnitude /= _peakMagnitudeList[BIN_INDEX];
            }

        } else {
            magnitude = magnitude / _absolutePeak;
        }

        magnitude *= 100;
    }
}
