/*
 * Analyzable.hpp
 *
 * Adapted from SFML/Music.hpp
 */

#ifndef ANALYZABLE_HPP
#define ANALYZABLE_HPP

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>

namespace sf
{
    class InputStream;

    class Analyzable : public SoundStream
    {
        public:
            /* 2.4
            template <typename T>
                    struct Span
                    {
                            Span()
                            {
                            }

                            Span(T off, T len):
                                    offset(off),
                                    length(len)
                            {
                            }

                            T offset;
                            T length;
                    };

            typedef Span<Time> TimeSpan;
            */

            Analyzable();

            ~Analyzable();

            bool openFromFile(const std::string& filename);

            bool openFromMemory(const void* data,
                            std::size_t sizeInBytes);

            bool openFromStream(InputStream& stream);

            Time getDuration() const;

            /* 2.4
            TimeSpan getLoopPoints() const;

            void setLoopPoints(TimeSpan timePoints);
            */

        protected:
            virtual bool onGetData(Chunk& data);

            virtual void onSeek(Time timeOffset);

            /* 2.4
            virtual Int64 onLoop();
            */

        private:

            void initialize();

            /* 2.4
            Uint64 timeToSamples(Time position) const;

            Time samplesToTime(Uint64 samples) const;
            */

            /* 2.4
            InputSoundFile     m_file;
            std::vector<Int16> m_samples;
            Mutex              m_mutex;
            Span<Uint64>       m_loopSpan;
            */

            InputSoundFile     m_file;
            Time               m_duration;
            std::vector<Int16> m_samples;
            Mutex              m_mutex;

    };
}

#endif
