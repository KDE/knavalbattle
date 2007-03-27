/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef HITINFO_H
#define HITINFO_H

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
    
    HitInfo(Type type, Ship* shipDestroyed = 0)
    : type(type)
    , shipDestroyed(shipDestroyed)
    {
    }
};

    
#endif // HITINFO_H

