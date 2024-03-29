/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ship.h"

#include "knavalbattle_debug.h"

Ship::Ship(unsigned int size, Direction direction, Coord position)
: m_size(size)
, m_position(position)
, m_direction(direction)
{
    m_life = size;
}

const Coord Ship::position() const
{
    return m_position;
}

void Ship::setPosition(const Coord & position)
{
    m_position = position;
}

void Ship::changeDirection()
{
    m_direction = m_direction == TOP_DOWN ? LEFT_TO_RIGHT : TOP_DOWN;
}

Coord Ship::increment(Direction direction)
{
    return direction == TOP_DOWN ? Coord(0, 1) : Coord(1, 0);
}

Coord Ship::decrement(Direction direction)
{
    return direction == TOP_DOWN ? Coord(0, -1) : Coord(-1, 0);
}

Coord Ship::incrementPerpendicular(Direction direction)
{
    return direction == TOP_DOWN ? Coord(1, 0) : Coord(0, 1);
}

Coord Ship::decrementPerpendicular(Direction direction)
{
    return direction == TOP_DOWN ? Coord(-1, 0) : Coord(0, -1);
}

Coord Ship::increment() const
{
    return increment(m_direction);
}

Coord Ship::decrement() const
{
    return decrement(m_direction);
}

Coord Ship::incrementPerpendicular() const
{
    return incrementPerpendicular(m_direction);
}

Coord Ship::decrementPerpendicular() const
{
    return decrementPerpendicular(m_direction);
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


