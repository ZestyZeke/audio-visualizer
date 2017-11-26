/*
 * audio-vis.cpp
 */
#include <iostream>
#include <SFML/Audio.hpp>
#include <unistd.h>
#include "analyzable.hpp"

int main(int argc, char **argv)
{
        sf::Music music;
        if (!music.openFromFile("res/Kingpin.wav"))
                return -1;
        music.play();
        usleep(music.getDuration().asMicroseconds());
        music.pause();

        std::cout << "Going to print random data about stream.\n";
        std::cout << "getDuration() == " << music.getDuration().asSeconds()
                << "\n";
        std::cout << "getChannelCount() == " << music.getChannelCount() << "\n";
        std::cout << "getSampleRate() == " << music.getSampleRate() << "\n";
        std::cout << "getPitch() == " << music.getPitch() << "\n";
        std::cout << "getVolume() == " << music.getVolume() << "\n";

        music.stop();

        sf::Analyzable test;
        if (!test.openFromFile("res/Kingpin.wav"))
                return -1;
        std::cout << "getDuration() == " << test.getDuration().asSeconds()
                << "\n";
        return 0;
}
