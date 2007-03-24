#include "element.h"
#include "ship.h"

Element::Element()
: m_parent(0)
, m_type(WATER)
{
}

Element::Element(Ship* parent)
: m_parent(parent)
, m_type(ALIVE)
{

}

bool Element::free() const 
{ 
    return m_type == ALIVE || m_type == WATER;
}

bool Element::water() const
{
    return m_type == WATER;
}

HitInfo::Type Element::hit()
{
    switch (m_type) {
    case ALIVE:
        m_type = DEAD;
        m_parent->decLife();
        return HitInfo::HIT;
    case WATER:
        m_type = MISS;
        return HitInfo::MISS;
    default:
        return HitInfo::INVALID;
    }
}

