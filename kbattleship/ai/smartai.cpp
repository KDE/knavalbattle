#include <algorithm>

// for rand
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "smartai.h"

class Strategy
{
protected:
    Sea::Player m_player;
    Sea* m_sea;
    SmartAI::State& m_state;
public:
    Strategy(Sea::Player player, Sea* sea, SmartAI::State& state)
    : m_player(player)
    , m_sea(sea)
    , m_state(state)
    {
    }
    
    virtual ~Strategy() { }
    virtual Coord getMove() = 0;
    virtual Strategy* notify(const Coord& c, const HitInfo& info) = 0;
};


class DestroyStrategy : public Strategy
{
    Coord m_begin;
    Coord m_end;
    int m_direction;
    
    inline Coord direction() const
    { 
        switch(m_direction) {
        case 0:
            return Coord(1, 0);
        case 1:
            return Coord(0, 1);
        case 2:
            return Coord(-1, 0);
        case 3:
        default:
            return Coord(0, -1);
        }
    }
    
    bool next_try()
    {
        // if it only hit once
        if (m_begin == m_end) {
            // change direction
            m_direction++;
            
            if (m_direction > 3) {
                return false; // no more to do
            }
        }
        else {
            if (m_direction > 1) {
                // no more to do on this line
                // we have probably hit more than one ship
                // in the process, so start over changing direction            
                m_end = m_begin;
                m_direction--;
            }
            else {
                // change direction on the same line
                m_direction += 2; 
                
                // swap begin and end
                std::swap(m_begin, m_end);
            }
        }
        
        return true;
    }
public:
    DestroyStrategy(Sea::Player player, Sea* sea, SmartAI::State& state, const Coord& begin)
    : Strategy(player, sea, state)
    , m_begin(begin)
    , m_end(begin)
    , m_direction(0)
    {
    }
    
    virtual Coord getMove()
    {
        for (;;) {
            Coord c = m_end + direction();
            while (m_sea->at(Sea::opponent(m_player), c).type() == Element::DEAD) {
                // there's already a hit: go on!
                c += direction();
            }
            if (m_sea->canHit(m_player, c)) {
                return c;
            }
            if (!next_try()) {
                return Coord::invalid();
            }
        }
    }
    
    Strategy* notify(const Coord& c, const HitInfo& info)
    {
        if (info.shipDestroyed) {
            // success!
            kDebug() << "destroyed!" << endl;
            Coord delta = m_end - m_begin;
            int size = abs(delta.x + delta.y);
            m_state.destroyed(size);
            return m_state.defaultStrategy(m_player, m_sea);
        }
        else if (info.type == HitInfo::HIT) {
            kDebug() << "hit" << endl;
            // hit, record info
            m_end = c;
        }
        else {
            kDebug() << "failure" << endl;
            if (!next_try()) {
                // give up
                kDebug() << "giving up (m_direction = " << m_direction << ")" << endl;
                return m_state.defaultStrategy(m_player, m_sea);
            }
        }
        
        return 0;
    }
};

class RandomStrategy : public Strategy
{
public:
    RandomStrategy(Sea::Player player, Sea* sea, SmartAI::State& state)
    : Strategy(player, sea, state)
    {
    }
      
    virtual Coord getMove()
    {
        for (int i = 0; i < 10000; i++) {
            Coord c(rand() % m_sea->size().x, rand() % m_sea->size().y);
            if (m_sea->canHit(m_player, c)) {
                return c;
            }
        }
        return Coord::invalid();
    }
    
    virtual Strategy* notify(const Coord& c, const HitInfo& info)
    {
        if (info.type == HitInfo::HIT &&
            !info.shipDestroyed) {
            // non-fatal hit, destroy ship now
            return new DestroyStrategy(m_player, m_sea, m_state, c);
        }
        else {
            return 0;
        }
    }
};


SmartAI::SmartAI(Sea::Player player, Sea* sea)
: AI(player, sea)
{
    srand(time(0));
    m_strategy = std::auto_ptr<Strategy>(m_state.defaultStrategy(player, sea));
}

Coord SmartAI::getMove()
{
    if (m_strategy.get() &&
        m_sea->turn() == m_player &&
        m_sea->status() == Sea::PLAYING) {
        return m_strategy->getMove();
    }
    else {
        return Coord::invalid();
    }
}

void SmartAI::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    if (player == m_player) {
        Strategy* new_strategy = m_strategy->notify(c, info);
        if (new_strategy) {
            m_strategy = std::auto_ptr<Strategy>(new_strategy);
        }
    }
}

void SmartAI::setShips()
{
    for (int i = 1; i <= 4; i++) {
        Ship* ship = 0;
        while (ship == 0) {
            Coord c(rand() % m_sea->size().x, rand() % m_sea->size().y);
            Ship::Direction dir = rand() % 2 == 0 ? Ship::LEFT_TO_RIGHT : Ship::TOP_DOWN;
            if (m_sea->canAddShip(m_player, c, i, dir)) {
                ship = new Ship(i, dir);
                m_sea->add(m_player, c, ship);
            }
        }
    }
}


SmartAI::State::State()
: m_largest(4)
{
}

Strategy* SmartAI::State::defaultStrategy(Sea::Player player, Sea* sea)
{
    return new RandomStrategy(player, sea, *this);
}

void SmartAI::State::destroyed(int size)
{
    if (size >= m_largest) {
        m_largest--;
    }
}




