/*
 * audio-vis.cpp
 */
#include <iostream>
#include <experimental/filesystem>

#include <aquila/global.h>

#include "Engine.h"
#include "Config.h"

int main(int argc, char **argv) {
    // don't sync streams since we're not concerned with printing output
    std::ios::sync_with_stdio(false);

    Config config;
    config.loadFile("config.xml");

    std::string fileName {"444.wav"};
    if (argc == 2) {
        fileName = std::string(argv[1]);
    }

    // verify that file exists
    if (std::experimental::filesystem::exists(fileName)) {
        Engine engine(fileName, config);
        engine.run();
    } else {
        std::cerr << "file [" << fileName <<
        "] does not exist in current directory" << std::endl;
    }
}
