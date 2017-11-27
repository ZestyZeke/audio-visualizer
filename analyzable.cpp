/*
 * analyzable.cpp
 */

#include "analyzable.hpp"
#include <fstream>

namespace sf
{
        Analyzable::Analyzable() :
                m_file (),
                m_loopSpan (0, 0)
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
                return m_file.getDuration();
        }

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

        bool Analyzable::onGetData(SoundStream::Chunk& data)
        {
                Lock lock(m_mutex);

                std::size_t toFill = m_samples.size();
                Uint64 currentOffset = m_file.getSampleOffset();
        }
}
