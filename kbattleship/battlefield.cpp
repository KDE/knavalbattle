#include <kdebug.h>

#include "battlefield.h"
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
    get(pos) = e;
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
    }

    return res;
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

#include "battlefield.moc"
