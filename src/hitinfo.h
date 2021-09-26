/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef HITINFO_H
#define HITINFO_H

#include "coord.h"

class Ship;

struct HitInfo
{
    enum Type
    {
        HIT,
        MISS,
        INVALID
    };
    
    Type type;
    Ship* shipDestroyed;
    Coord shipPos;
    
    HitInfo(Type type)
    : type(type)
    , shipDestroyed(nullptr)
    {
    }
    
    HitInfo(Type type, Ship* shipDestroyed, const Coord& shipPos)
    : type(type)
    , shipDestroyed(shipDestroyed)
    , shipPos(shipPos)
    {
    }
};

    
#endif // HITINFO_H

