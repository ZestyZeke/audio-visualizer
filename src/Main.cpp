/*
 * audio-vis.cpp
 */
#include <iostream>
#include <SFML/Audio.hpp>
#include <unistd.h>
#include "Analyzable.hpp"
#include <fftw3.h>
#include <aquila/global.h>
#include <aquila/source/generator/SineGenerator.h>
#include <aquila/tools/TextPlot.h>
#include <aquila/source/WaveFile.h>

int main(int argc, char **argv)
{
    /*
        Aquila::WaveFile wav("444.wav");
        std::cout << "Loaded file: " << wav.getFilename();
        std::cout << " (" << wav.getBitsPerSample() << "b)" << std::endl;
        Aquila::SampleType maxValue = 0, minValue = 0, average = 0;

        std::cout << "NumSamples: " << wav.getSamplesCount() << std::endl;
        int i = 0;
        for (auto sample : wav) {
            if (sample > maxValue)
                maxValue = sample;
            //std::cout << i++ << std::endl;
        }
        std::cout << "Max sample value: " << maxValue << std::endl;
        std::cout << "Audio length: " << wav.getAudioLength() << std::endl;
        */
        /*
        std::cout << Aquila::VERSION_STRING << std::endl;
        Aquila::SineGenerator generator(1000);
        generator.setFrequency(32).setAmplitude(255).generate(64);
        Aquila::TextPlot plot("Sine wave");
        plot.plot(generator);

        generator.setFrequency(50).generate(32);
        plot.plot(generator);
         */
        /*
        //@NOTE: let in == out, to transform in-place.
        //@NOTE: try doing r2r transform instead.
        static const std::size_t N = 10;
        fftw_complex *in, *out;
        fftw_plan p;
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        fftw_destroy_plan(p);
        fftw_free(in);
        fftw_free(out);
         */
        /*
        sf::Analyzable song;
        if (!song.openFromFile("Kingpin.wav"))
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
         */

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
