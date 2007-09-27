/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "battlefield.h"

#include <kdebug.h>

#include "sea.h"

BattleField::BattleField(Sea* parent, const Coord& size)
: QObject(parent)
, m_size(size)
, m_board(size)
, m_ships(0)
{
}

BattleField::~BattleField()
{
	QSet<Ship*> deleted_ships;
	FOREACH_SQUARE(p, m_board) {
		Ship* ship = m_board[p].parent();
		if (ship && !deleted_ships.contains(ship)) {
			delete ship;
			deleted_ships.insert(ship);
		}
	}
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

void BattleField::add(const Coord& pos, Ship* ship)
{
    Coord p = pos;
    for (unsigned int i = 0; i < ship->size(); i++) {
        set(p, Element(ship));
        p = p + ship->increment();
    }
    m_ships++;
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

bool BattleField::canAddShip(const Coord& pos, unsigned int size, Ship::Direction direction) const
{
    Coord p = pos;
    Coord inc = Ship::increment(direction);
    for (unsigned int i = 0; i < size; i++) {
        if (!valid(p))
            return false;
        if (!get(p).water())
            return false;
        p += inc;
    }
    return true;
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

#include "battlefield.moc"
