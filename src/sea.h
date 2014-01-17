/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef Sea_H
#define Sea_H

#include <QList>
#include <QObject>

#include "ship.h"
#include "ships.h"
#include "hitinfo.h"

class BattleField;

class Sea : public QObject
{
Q_OBJECT
public:
    enum Status
    {
        PLACING_SHIPS,
        PLAYING,
        A_WINS,
        B_WINS
    };
    enum Player
    {
        PLAYER_A = 0,
        PLAYER_B = 1,
        NO_PLAYER = -1
    };
private:
    Coord m_size;
    Player m_turn;
    BattleField* m_fields[2];
    QList<Ship *> m_enemyShips;
    QList<Ship *> m_myShips;
    Status m_status;
    BattleShipsConfiguration m_battle_ships_configuration;

    inline BattleField* currentField() const { return m_fields[m_turn]; }
    inline BattleField* otherField() const { return m_fields[opponent(m_turn)]; }

    void checkGameOver();
public:
    Sea(QObject* parent, const BattleShipsConfiguration battleShipsConfiguration);
    ~Sea();

    bool canAddShip(Player p, const Coord& pos, int size, Ship::Direction direction) const;
    bool canAddShipOfSize(Player p, int size) const;
    void add(Player p, int n);
    void add(Player p, Ship* ship);
    void addBorder(Player p, const Coord& pos);
    bool canHit(Player p, const Coord& pos) const;
    HitInfo hit(const Coord& pos);
    void forceHit(const Coord& pos, const HitInfo& info);
    void startPlaying();
    void abort(Player p);
    const Element& at(Sea::Player player, const Coord& pos) const;
    bool valid(Sea::Player, const Coord& pos) const;
    void switchTurn();
    bool isNearShip(Sea::Player, const Coord& pos) const;
    void allowAdjacentShips(const bool allow_adjacent_ships);
    void clear(Sea::Player);

    const QList<Ship *> enemyShips() const;
    const QList<Ship *> myShips() const;

    inline Status status() const { return m_status; }
    inline Player turn() const { return m_turn; }
    static Player opponent(Player p);
    inline Coord size() const { return m_size; }
    inline bool isAdjacentShipsAllowed() const { return m_battle_ships_configuration.isAllowedAdjacentShips(); }
    inline bool isSeveralShipsAllowed() const { return m_battle_ships_configuration.multipleShips(); }
    inline const BattleShipsConfiguration* battleShipsConfiguration() const { return &m_battle_ships_configuration; }
};

#endif // Sea_H

