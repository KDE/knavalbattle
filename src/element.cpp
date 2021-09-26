/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "element.h"
#include "ship.h"

Element::Element(Type type)
: m_parent(nullptr)
, m_type(type)
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

