/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AI__AI_H
#define AI__AI_H

#include "sea.h"

class AI
{
protected:
    Sea::Player m_player;
    Sea* m_sea;
public:
    AI(Sea::Player player, Sea* sea);
    virtual ~AI() { }
    virtual Coord getMove() = 0;
    virtual void setShips() = 0;
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& hit) = 0;
    Coord desperateMove() const;
};

#endif // AI__AI_H

