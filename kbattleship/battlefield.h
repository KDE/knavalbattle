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
    unsigned int m_ships;
    
    inline int convert(const Coord& c) const { return c.x + m_size.x * c.y; }
public:
    BattleField(Sea* parent, const Coord& size);
    ~BattleField();
    
    bool valid(const Coord& pos) const;
    Element& get(const Coord& pos);
    const Element& get(const Coord& pos) const;
    void set(const Coord& pos, const Element& e);
    
    void add(const Coord& pos, Ship* ship);
    bool canAddShip(const Coord& pos, unsigned int size, Ship::Direction direction) const;
    HitInfo hit(const Coord& pos);
    Coord find(Ship* ship) const;
    
    inline unsigned int ships() const { return m_ships; }
signals:
    void shipDestroyed(Ship*);
};

#endif // BATTLEFIELD_H

