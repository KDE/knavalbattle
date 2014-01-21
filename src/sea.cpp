/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "sea.h"
#include "battlefield.h"

Sea::Sea(QObject* parent, const BattleShipsConfiguration& battleShipsConfiguration)
: QObject(parent)
, m_size(Coord(battleShipsConfiguration.boardWidth(), battleShipsConfiguration.boardHeight()))
, m_turn(PLAYER_A)
, m_status(PLACING_SHIPS)
, m_battle_ships_configuration(battleShipsConfiguration)
{
    m_fields[0] = new BattleField(this, m_size, battleShipsConfiguration.isAllowedAdjacentShips());
    m_fields[1] = new BattleField(this, m_size, battleShipsConfiguration.isAllowedAdjacentShips());
}

Sea::~Sea()
{
    m_enemyShips.clear();
    m_myShips.clear();

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

bool Sea::canAddShipOfSize(Player p, int size) const
{
    if (m_status != PLACING_SHIPS) {
        return false;
    }
    return m_fields[p]->canAddShipOfSize(size);
}

void Sea::clear(Sea::Player p)
{
    if (m_status == PLACING_SHIPS) {
        m_fields[p]->clear();
        if(p == PLAYER_B) {
            m_enemyShips.clear();
        }
        else {
            m_myShips.clear();
        }
    }
}

void Sea::add(Player p, Ship* ship)
{
    if(p == PLAYER_B) {
        m_enemyShips.append(ship);
    }
    else {
        m_myShips.append(ship);
    }

    m_fields[p]->add(ship);
}

void Sea::add(Player p, int n)
{
    m_fields[p]->add(n);
}

void Sea::addBorder(Player p, const Coord& pos)
{
    m_fields[p]->addBorder(pos);
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
    checkGameOver();
    return res;
}

void Sea::forceHit(const Coord& pos, const HitInfo& info)
{
    otherField()->forceHit(pos, info);
    checkGameOver();
}

void Sea::checkGameOver()
{
    if (otherField()->ships() <= 0) {
        m_status = m_turn == PLAYER_A ? A_WINS : B_WINS;
    }
    else {
        switchTurn();
    }
}

void Sea::allowAdjacentShips(const bool allow_adjacent_ships)
{
    m_battle_ships_configuration.setAllowAdjacentShips(allow_adjacent_ships);
    m_fields[0]->setAllowAdjacentShips(allow_adjacent_ships);
    m_fields[1]->setAllowAdjacentShips(allow_adjacent_ships);
}

const Element& Sea::at(Sea::Player player, const Coord& c) const
{
    return m_fields[player]->at(c);
}

bool Sea::valid(Sea::Player player, const Coord& pos) const
{
    return m_fields[player]->valid(pos);
}

// Coord Sea::find(Player p, Ship* ship) const
// {
//     return m_fields[p]->find(ship);
// }

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

bool Sea::isNearShip(Sea::Player p, const Coord& pos) const
{
    return m_fields[p]->isNearShip(pos);
}

const QList<Ship *> Sea::enemyShips() const
{
    return m_enemyShips;
}

const QList<Ship *> Sea::myShips() const
{
    return m_myShips;
}

void Sea::setBattleShipsConfiguration(const BattleShipsConfiguration& configuration)
{
    m_battle_ships_configuration=configuration;
}


#include "sea.moc"

