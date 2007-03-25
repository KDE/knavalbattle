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

#include "sea.moc"

