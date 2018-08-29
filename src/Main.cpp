/*
 * audio-vis.cpp
 */
#include <iostream>
#include <SFML/Audio.hpp>
#include <unistd.h>
#include "Analyzable.hpp"

int main(int argc, char **argv)
{
        sf::Analyzable song;
        if (!song.openFromFile("./res/Kingpin.wav"))
                return -1;
        song.play();
        usleep(song.getDuration().asMicroseconds());
        song.pause();

        std::cout << "Going to print random data about stream.\n";
        std::cout << "getDuration() == " << song.getDuration().asSeconds()
                << "\n";
        std::cout << "getChannelCount() == " << song.getChannelCount() << "\n";
        std::cout << "getSampleRate() == " << song.getSampleRate() << "\n";
        std::cout << "getPitch() == " << song.getPitch() << "\n";
        std::cout << "getVolume() == " << song.getVolume() << "\n";

        song.stop();

        /*
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
                */
        return 0;
}
