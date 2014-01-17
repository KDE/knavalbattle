/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AI_DUMMYAI_H
#define AI_DUMMYAI_H

#include "ai.h"

class DummyAI : public AI
{
public:
    DummyAI(Sea::Player player, Sea* sea, const BattleShipsConfiguration* config);
    virtual Coord getMove();
    virtual void notify(Sea::Player, const Coord&, const HitInfo&) { }
};

#endif // AI_DUMMYAI_H

