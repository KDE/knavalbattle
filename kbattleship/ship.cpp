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
    kDebug() << "changing ship direction" << endl;
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


