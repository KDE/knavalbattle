/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
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

