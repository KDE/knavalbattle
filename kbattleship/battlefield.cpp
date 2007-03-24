#include <kdebug.h>

#include "battlefield.h"
#include "sea.h"

BattleField::BattleField(Sea* parent, const Coord& size)
: QObject(parent)
, m_size(size)
, m_ships(0)
{
    const unsigned int n = m_size.x * m_size.y;
    m_board = new Element[n];
}

BattleField::~BattleField()
{
    delete[] m_board;
}

bool BattleField::valid(const Coord& pos) const
{
    return pos.x >= 0 &&
        pos.x < m_size.x &&
        pos.y >= 0 &&
        pos.y < m_size.y;
}

Element& BattleField::get(const Coord& pos)
{
    Q_ASSERT(valid(pos));
    return m_board[convert(pos)];
}

const Element& BattleField::get(const Coord& pos) const
{
    Q_ASSERT(valid(pos));
    return m_board[convert(pos)];
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
        res.shipDestroyed = true;
    }

    return res;
}

#include "battlefield.moc"
