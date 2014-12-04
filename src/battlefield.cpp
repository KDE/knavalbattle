/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "battlefield.h"

#include "sea.h"
#include "settings.h"

BattleField::BattleField(Sea* parent, const Coord& size, const bool allow_adjacent_ships)
: QObject(parent)
, m_size(size)
, m_board(size)
, m_secondary_board(size)
, m_allow_adjacent_ships(allow_adjacent_ships)
, m_ships(0)
{
    setUpSecondaryBoard();
}

void BattleField::setUpSecondaryBoard()
{
    for (int i = 0; i < m_size.x; i++) {
        for (int j = 0; j < m_size.y ; j++) {
            m_secondary_board[Coord(i,j)] = false;
        }
    }
}

BattleField::~BattleField()
{
    clear();
}

bool BattleField::valid(const Coord& pos) const
{
    return m_board.valid(pos);
}

Element& BattleField::get(const Coord& pos)
{
    return m_board[pos];
}

const Element& BattleField::get(const Coord& pos) const
{
    return m_board[pos];
}

void BattleField::set(const Coord& pos, const Element& e)
{
    if (valid(pos)) {
        get(pos) = e;
    }
}

void BattleField::add(int n)
{
    m_ships += n;
}

void BattleField::add(Ship* ship)
{
    Coord p = ship->position();
    for (unsigned int i = 0; i < ship->size(); i++) {
        set(p, Element(ship));
        p = p + ship->increment();
    }
    m_ships++;

    addSecondaryBoard(ship);
    addBorderSecondaryBoard(ship);
}

void BattleField::addBorder(const Coord& pos)
{
    Ship* ship = get(pos).parent();
    if (ship) {
        Coord inc = ship->increment();
        Coord orth(inc.y, inc.x);
        Coord p = pos - inc;
        set(p, Element::BORDER);
        for (; p != pos + inc * (ship->size() + 1); p += inc) {
            set(p + orth, Element::BORDER);
            set(p - orth, Element::BORDER);
        }
        p -= inc;
        set(p, Element::BORDER);
    }
}

void BattleField::addSecondaryBoard(Ship* ship)
{
    Coord p = ship->position();
    for (unsigned int i = 0; i < ship->size(); i++) {
        m_secondary_board[p]=true;
        p += ship->increment();
    }
}

void BattleField::addBorderSecondaryBoard(Ship* ship)
{
    if ( !m_allow_adjacent_ships )
    {
        Coord inc = ship->increment();
        Coord orth(inc.y, inc.x);
        Coord p = ship->position() - inc;
        if ( valid (p) ) {
            m_secondary_board[p]=true;
        }
        for (; p != ship->position() + inc * (ship->size() + 1); p += inc) {
            if ( valid( p + orth ) ) {
                m_secondary_board[p + orth]=true;
            }
            if ( valid( p - orth ) ) {
                m_secondary_board[p - orth]=true;
            }
        }
        p -= inc;
        if ( valid (p) ) {
            m_secondary_board[p]=true;
        }
    }
}

bool BattleField::canAddShip(const Coord& pos, unsigned int size, Ship::Direction direction) const
{
    Coord p = pos;
    Coord inc = Ship::increment(direction);
    // Can not place a ship outside the battlefield
    for (unsigned int i = 0; i < size; i++) {
       if (!valid(p))
          return false;
       p += inc;
    }
    // nor over another ship
    if (m_allow_adjacent_ships) {
        p = pos;
        for (unsigned int i = 0; i < size; i++) {
            if (valid(p) && !get(p).water())
                return false;
            p += inc;
        }
    }
    else {
    // if not addjacent ships enabled, there must be
    // a space between the already placed ships
    // and the new ship
        p=pos + Ship::decrement(direction) + Ship::decrementPerpendicular(direction);
        for (unsigned int i = 0; i < size+2; i++) {
            if (valid(p) && !get(p).water())
                return false;
            p += inc;
        }
        p=pos + Ship::decrement(direction);
        for (unsigned int i = 0; i < size+2; i++) {
            if (valid(p) && !get(p).water())
                return false;
            p += inc;
        }
        p=pos + Ship::decrement(direction) + Ship::incrementPerpendicular(direction);
        for (unsigned int i = 0; i < size+2; i++) {
            if (valid(p) && !get(p).water())
                return false;
            p += inc;
        }
    }
    return true;
}

bool BattleField::canAddShipOfSizeInHorizontal(unsigned int size) const
{
    unsigned int maxLenAvailable = 0;
    for (int j=0; j<m_secondary_board.height(); j++) {
        unsigned int contiguousLen = 0;
        for (int i=0; i<m_secondary_board.width(); i++) {
            if ( m_secondary_board[ Coord(i,j) ] ) {
                contiguousLen = 0;
            }
            else {
                contiguousLen += 1;
                maxLenAvailable = qMax<int>( contiguousLen, maxLenAvailable );
                if (maxLenAvailable >= size)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool BattleField::canAddShipOfSizeInVertical(unsigned int size) const
{
    unsigned int maxLenAvailable = 0;
    for (int i=0; i<m_secondary_board.width(); i++) {
        unsigned int contiguousLen = 0;
        for (int j=0; j<m_secondary_board.height(); j++) {
            if ( m_secondary_board[ Coord(j,i) ] ) {
                contiguousLen = 0;
            }
            else {
                contiguousLen += 1;
                maxLenAvailable = qMax<int>( contiguousLen, maxLenAvailable );
                if (maxLenAvailable >= size)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool BattleField::canAddShipOfSize(unsigned int size) const
{
    return canAddShipOfSizeInHorizontal(size) || canAddShipOfSizeInVertical(size);
}


HitInfo BattleField::hit(const Coord& pos)
{
    Element& e = get(pos);
    HitInfo::Type type = e.hit();
    Ship* ship = e.parent();
    
    HitInfo res(type);
    if (ship && !ship->alive())
    {
        m_ships--;
        res.shipDestroyed = ship;
        res.shipPos = find(ship);
    }

    return res;
}

void BattleField::forceHit(const Coord& pos, const HitInfo& info)
{
    switch (info.type) {
    case HitInfo::HIT:
        get(pos).setType(Element::DEAD);
        if (info.shipDestroyed) {
            Coord c = info.shipPos;
            for (unsigned int i = 0; i < info.shipDestroyed->size(); i++) {
                get(c).setParent(info.shipDestroyed);
                c += info.shipDestroyed->increment();
            }
            m_ships--;
        }
        break;
    case HitInfo::MISS:
        get(pos).setType(Element::MISS);
        break;
    case HitInfo::INVALID:
        break;
    }
}

const Element& BattleField::at(const Coord& c) const
{
    return m_board[c];
}

Coord BattleField::find(Ship* ship) const
{
    FOREACH_SQUARE(p, m_board) {
        if (m_board[p].parent() == ship) {
            return p;
        }
    }
    return Coord::invalid();
}

bool BattleField::isNearShip(const Coord& pos) const
{
    for (int i = -1; i <= 1; i++)
    for (int j = -1; j <= 1; j++) {
        Coord p = pos + Coord(i,j);
        if (valid(p) && get(p).parent()) {
            return true;
        }
    }
    return false;
}

void BattleField::clear()
{
    for (int i = 0; i < m_size.x; i++) {
        for (int j = 0; j < m_size.y ; j++) {
            m_secondary_board[Coord(i,j)] = false;
        }
    }
    QSet<Ship*> deleted_ships;
    FOREACH_SQUARE(p, m_board) {
        Ship* ship = m_board[p].parent();
        if (ship && !deleted_ships.contains(ship)) {
            delete ship;
            deleted_ships.insert(ship);
        }
        m_board[p].setType(Element::WATER);
        m_board[p].setParent(0);
    }
    m_ships=0;
}



