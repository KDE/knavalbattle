/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "sea.h"
#include "battlefield.h"

Sea::Sea(QObject* parent, const Coord& size)
: QObject(parent)
, m_size(size)
, m_turn(PLAYER_A)
, m_status(PLACING_SHIPS)
{
    m_fields[0] = new BattleField(this, m_size);
    m_fields[1] = new BattleField(this, m_size);
}

Sea::~Sea()
{
	delete m_fields[0];
	delete m_fields[1];
}

bool Sea::canAddShip(Player p, const Coord& pos, int size, Ship::Direction direction) const
{
    if (m_status != PLACING_SHIPS) {
        return false;
    }
    return m_fields[p]->canAddShip(pos, size, direction);
}

void Sea::add(Player p, const Coord& pos, Ship* ship)
{
    m_fields[p]->add(pos, ship);
}

bool Sea::canHit(Player p, const Coord& pos) const
{
    if (m_status != PLAYING || m_turn != p) {
        return false;
    }
    if (!otherField()->valid(pos)) {
        return false;
    }
    Element& e = otherField()->get(pos);
    return e.free();
}

HitInfo Sea::hit(const Coord& pos)
{
    HitInfo res = otherField()->hit(pos);
    if (otherField()->ships() <= 0) {
        m_status = m_turn == PLAYER_A ? A_WINS : B_WINS;
    }
    else {
        switchTurn();
    }
    return res;
}

const Element& Sea::at(Sea::Player player, const Coord& c) const
{
    return m_fields[player]->at(c);
}

bool Sea::valid(Sea::Player player, const Coord& pos) const
{
    return m_fields[player]->valid(pos);
}

Coord Sea::find(Player p, Ship* ship) const
{
    return m_fields[p]->find(ship);
}

void Sea::switchTurn()
{
    m_turn = opponent(m_turn);
}

Sea::Player Sea::opponent(Player p)
{
    return p == PLAYER_A ? PLAYER_B : PLAYER_A;
}

void Sea::startPlaying()
{
    Q_ASSERT(m_status == PLACING_SHIPS);
    m_status = PLAYING;
}

void Sea::abort(Player p)
{
    if (p == PLAYER_A) {
        m_status = A_WINS;
    }
    else {
        m_status = B_WINS;
    }
}

#include "sea.moc"

