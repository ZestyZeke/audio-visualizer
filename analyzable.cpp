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
                // left off here.
        }
}
