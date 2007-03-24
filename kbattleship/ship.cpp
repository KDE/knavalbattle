#include "ship.h"
#include <qdebug.h>

Ship::Ship(unsigned int size, Direction direction)
: m_size(size)
, m_direction(direction)
{
    m_life = size;
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


