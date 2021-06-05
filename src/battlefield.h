/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "ship.h"
#include "hitinfo.h"
#include "grid.h"
#include <QObject>

class Sea;

class BattleField : public QObject
{
Q_OBJECT
    typedef Grid<Element> Board;
    Coord m_size;
    Board m_board;
    // Using a second board to annotate the real space used by the ships
    // Another solution would be to annotate the borders along with the ships,
    // and then send a signal to clean the borders when the shooting is going to start.
    Grid<bool> m_secondary_board;
    bool m_allow_adjacent_ships;
    unsigned int m_ships;

    inline int convert(const Coord& c) const { return c.x + m_size.x * c.y; }
    void setUpSecondaryBoard();
    void addSecondaryBoard(Ship* ship);
    void addBorderSecondaryBoard(Ship *ship);
    bool canAddShipOfSizeInHorizontal(unsigned int size) const;
    bool canAddShipOfSizeInVertical(unsigned int size) const;
public:
    BattleField(Sea* parent, const Coord& size, const bool allow_adjacent_ships);
    ~BattleField() override;

    bool valid(const Coord& pos) const;
    Element& get(const Coord& pos);
    const Element& get(const Coord& pos) const;
    void set(const Coord& pos, const Element& e);

    void add(Ship* ship);
    void add(int n);
    void addBorder(const Coord& pos);
    bool canAddShip(const Coord& pos, unsigned int size, Ship::Direction direction) const;
    bool canAddShipOfSize(unsigned int size) const;
    HitInfo hit(const Coord& pos);
    void forceHit(const Coord& pos, const HitInfo& info);
    const Element& at(const Coord& c) const;
    Coord find(Ship* ship) const;
    bool isNearShip(const Coord& c) const;
    void setAllowAdjacentShips(const bool adjacent) { m_allow_adjacent_ships = adjacent; };
    void clear(); // to start placing the ships again in a clean BattleField
    inline unsigned int ships() const { return m_ships; }
signals:
    void shipDestroyed(Ship*);
};

#endif // BATTLEFIELD_H

