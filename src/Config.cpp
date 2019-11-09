//
// Created by zeke on 2/24/19.
//

#include "Config.h"
#include <pugixml.hpp>
#include <iostream>
#include <algorithm>
#include <range/v3/all.hpp>

void Config::loadFile(std::string fileName) {
    using namespace pugi;
    using namespace std::string_literals;

    xml_document doc;
    xml_parse_result result = doc.load_file(fileName.c_str());
    if (result.status != xml_parse_status::status_ok) {
        std::cerr << "error parsing config file: " << fileName << std::endl;
        return;
    }

    xml_node root = doc.first_child(); // should be 'config'

    // naive xml tree parse
    constexpr size_t NUM_FEATURES = 8;
    std::bitset<NUM_FEATURES> count {false};
    for (xml_node node : root.children()) {
        if (node.name() == "fftSize"s) {
            fftSize = std::atoi(node.child_value());
            count[0] = true;


        } else if (node.name() == "resolution"s) {
            xml_node childX = node.child("x");
            resolution.x = std::atoi(childX.child_value());
            count[1] = true;

            xml_node childY = node.child("y");
            resolution.y = std::atoi(childY.child_value());
            count[2] = true;

        } else if (node.name() == "maxHeight"s) {
            maxHeight = std::atoi(node.child_value());
            count[3] = true;

        } else if (node.name() == "numRows"s) {
            numRows = std::atoi(node.child_value());
            count[4] = true;

        } else if (node.name() == "windowTitle"s) {
            // remove whitespace
            // have to overload isspace... else behavior is undefined
            auto isSpace = [](unsigned char c) { return std::isspace(c); };

            using namespace ranges;
            windowTitle = std::string(node.child_value()) | action::remove_if(isSpace);

            count[5] = true;

        } else if (node.name() == "ewmaAlpha"s) {
            ewmaAlpha = std::atof(node.child_value());
            count[6] = true;

        } else if (node.name() == "useSimpleScale"s) {
            useSimpleScale = std::atoi(node.child_value()) == 1;
            count[7] = true;
        }
    }

    if (!count.all()) {
        std::cerr << "error parsing elements of config file: " <<
            fileName << std::endl;
    }
}