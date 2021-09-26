/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    Coord m_position;
    Direction m_direction;
    int m_life;
public:
    Ship(unsigned int size, Direction direction, Coord position);

    void decLife();
    void setPosition(const Coord & position);
    void changeDirection();

    const Coord position() const;
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

    bool alive() const;
};

#endif // Ship_H
