/*
 * analyzable.cpp
 */

#include "analyzable.hpp"

namespace sf
{
        bool Analyzable::onGetData(Chunk& data)
        {
                return false;
        }

        void Analyzable::onSeek(Time timeOffset)
        {
                return;
        }

        Int64 Analyzable::onLoop()
        {
                return 0;
        }
}
