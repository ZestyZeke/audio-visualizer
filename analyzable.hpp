/*
 * analyzable.hpp
 */

#ifndef ANALYZABLE_HPP
#define ANALYZABLE_HPP

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>

namespace sf
{
        class Analyzable : public Music
        {
                virtual bool onGetData(Chunk& data);

                virtual void onSeek(Time timeOffset);

                virtual Int64 onLoop();
        };
}

#endif
