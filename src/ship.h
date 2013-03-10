/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef Ship_H
#define Ship_H

#include "coord.h"
#include "element.h"

class Ship
{
public:
    enum Direction
    {
        TOP_DOWN,
        LEFT_TO_RIGHT
    };
private:
    unsigned int m_size;
    Direction m_direction;
    int m_life;
public:
    Ship(unsigned int size, Direction direction);

    void changeDirection();
    Coord increment() const;
    Coord decrement() const;
    Coord incrementPerpendicular() const;
    Coord decrementPerpendicular() const;
    static Coord increment(Direction);
    static Coord decrement(Direction);
    static Coord incrementPerpendicular(Direction);
    static Coord decrementPerpendicular(Direction);
    inline unsigned int size() const { return m_size; }
    inline Direction direction() const { return m_direction; }
    void decLife();
    bool alive() const;
};

#endif // Ship_H
