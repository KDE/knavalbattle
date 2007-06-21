/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef SHOT_H
#define SHOT_H

#include "sea.h"

class Controller;

class Shot
{
    Controller* m_parent;
    Sea::Player m_player;
    Coord m_pos;
public:
    Shot(Controller* parent, Sea::Player player, const Coord& pos);
    void execute(const HitInfo& info);
    
    Sea::Player player() const { return m_player; }
    const Coord& pos() const { return m_pos; }
};


#endif // SHOT_H
