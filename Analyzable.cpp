/*
 * analyzable.cpp
 */

#include "Analyzable.hpp"
#include <fstream>
#include <iostream>
#include "math.h"

namespace sf
{
        Analyzable::Analyzable() :
                m_file (),
                /* 2.4
                m_loopSpan (0, 0)
                */
                m_duration()
        {
        }

        Analyzable::~Analyzable()
        {
                stop();
        }

        bool Analyzable::openFromFile(const std::string& filename)
        {
                stop();

                if (!m_file.openFromFile(filename))
                        return false;

                initialize();

                return true;
        }

        bool Analyzable::openFromMemory(const void* data, std::size_t sizeInBytes)
        {
                stop();

                if (!m_file.openFromMemory(data, sizeInBytes))
                        return false;

                initialize();

                return true;
        }

        bool Analyzable::openFromStream(InputStream& stream)
        {
                stop();

                if (!m_file.openFromStream(stream))
                        return false;

                initialize();

                return true;
        }

        Time Analyzable::getDuration() const
        {
                /* 2.4
                return m_file.getDuration();
                */
                return m_duration;
        }

        /* 2.4
        Analyzable::TimeSpan Analyzable::getLoopPoints() const
        {
                return TimeSpan(samplesToTime(m_loopSpan.offset),
                                samplesToTime(m_loopSpan.length));
        }

        void Analyzable::setLoopPoints(TimeSpan timePoints)
        {
                Span<Uint64> samplePoints(timeToSamples(timePoints.offset),
                                timeToSamples(timePoints.length));
                if (getChannelCount() == 0 || m_file.getSampleCount() == 0) {
                        sf::err() << "Analyzable is not in a valid state to";
                        sf::err() << " assign Loop Points." << std::endl;
                        return;
                }

                samplePoints.offset += (getChannelCount() - 1);
                samplePoints.offset += (samplePoints.offset % getChannelCount());
                samplePoints.length += (getChannelCount() - 1);
                samplePoints.length += (samplePoints.length % getChannelCount());

                if (samplePoints.offset >= m_file.getSampleCount()) {
                        sf::err() << "LoopPoints offset val must be in range";
                        sf::err() << " [0, Duration)." << std::endl;
                        return;
                }
                if (samplePoints.length == 0) {
                        sf::err() << "LoopPoints length val must be nonzero.";
                        sf::err() << std::endl;
                        return;
                }

                samplePoints.length = std::min(samplePoints.length,
                                m_file.getSampleCount() - samplePoints.offset);

                if (samplePoints.offset == m_loopSpan.offset && samplePoints.length == m_loopSpan.length)
                        return;

                Status oldStatus = getStatus();
                Time oldPos = getPlayingOffset();

                stop();

                m_loopSpan = samplePoints;

                if (oldPos != Time::Zero)
                        setPlayingOffset(oldPos);

                if (oldStatus == Playing)
                        play();
        }
        */

        /* 2.4
        bool Analyzable::onGetData(SoundStream::Chunk& data)
        {
                Lock lock(m_mutex);

                std::size_t toFill = m_samples.size();
                Uint64 currentOffset = m_file.getSampleOffset();
                Uint64 loopEnd = m_loopSppan.offset + m_loopSpan.length;
                // left off here bc noticed version mismatch on github (2.4)
        }
        */

        bool Analyzable::onGetData(SoundStream::Chunk& data)
        {
                Lock lock(m_mutex);

                data.samples     = &m_samples[0];
                data.sampleCount = static_cast<std::size_t>(m_file.read(
                                        &m_samples[0], m_samples.size()));

                //std::cout << "Going to print samples: ";
                for (std::size_t i = 0; i < data.sampleCount / 1000
                        /*data.sampleCount / 8000*/; i += 1000) {
                    //std::cout << data.samples[i] << " ";
                    for (auto j = 0; j < data.samples[i] / 10; j++)
                        std::cout << "*";
                    std::cout << std::endl;
                }
                std::cout << "\n";

                //std::cout << "data.samples data.sampleCount\n";
                //std::cout << *data.samples << " " << data.sampleCount << "\n";
                return data.sampleCount == m_samples.size();
        }

        void Analyzable::onSeek(Time timeOffset)
        {
                Lock lock(m_mutex);

                m_file.seek(timeOffset);
        }

        void Analyzable::initialize()
        {
                m_duration = m_file.getDuration();

                m_samples.resize(m_file.getSampleRate() * m_file.getChannelCount());

                SoundStream::initialize(m_file.getChannelCount(),
                                m_file.getSampleRate());
        }
}
