/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "ship.h"
#include <qdebug.h>

Ship::Ship(unsigned int size, Direction direction)
: m_size(size)
, m_direction(direction)
{
    m_life = size;
}

void Ship::changeDirection()
{
    m_direction = m_direction == TOP_DOWN ? LEFT_TO_RIGHT : TOP_DOWN;
}

Coord Ship::increment(Direction direction)
{
    return direction == TOP_DOWN ? Coord(0, 1) : Coord(1, 0);
}

Coord Ship::increment() const
{
    return increment(m_direction);
}

void Ship::decLife()
{
    Q_ASSERT(alive());
    m_life--;
}

bool Ship::alive() const
{
    return m_life > 0;
}


