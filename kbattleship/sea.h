/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef Sea_H
#define Sea_H

#include <QObject>
#include "ship.h"
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
        PLAYER_B = 1
    };
private:
    Coord m_size;
    Player m_turn;
    BattleField* m_fields[2];
    Status m_status;
    
    void switchTurn();
    inline BattleField* currentField() const { return m_fields[m_turn]; }
    inline BattleField* otherField() const { return m_fields[opponent(m_turn)]; }
public:
    Sea(QObject* parent, const Coord& size);
    ~Sea();
    
    bool canAddShip(Player p, const Coord& pos, int size, Ship::Direction direction) const;
    void add(Player p, const Coord& pos, Ship* ship);
    bool canHit(Player p, const Coord& pos) const;
    HitInfo hit(const Coord& pos);
    void startPlaying();
    void abort(Player p);
    Coord find(Player p, Ship* ship) const;
    
    inline Status status() const { return m_status; }
    inline Player turn() const { return m_turn; }
    static Player opponent(Player p);
    inline Coord size() const { return m_size; }
};

#endif // Sea_H

