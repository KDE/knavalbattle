/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "smartai.h"

#include <algorithm>

// for rand
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

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
    Coord m_original;
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
                m_begin = m_original;
                m_end = m_original;
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
    , m_original(begin)
    , m_begin(begin)
    , m_end(begin)
    , m_direction(0)
    {
    }
    
    virtual Coord getMove()
    {
        for (;;) {
            Coord c = m_end + direction();
            Sea::Player opp = Sea::opponent(m_player);
            while (m_sea->valid(opp, c) && m_sea->at(opp, c).type() == Element::DEAD) {
                // there's already a hit: go on!
                c += direction();
            }
            if (m_sea->valid(opp, c) && m_sea->canHit(m_player, c)) {
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
            m_state.destroyed(info.shipDestroyed->size());
            return m_state.defaultStrategy(m_player, m_sea);
        }
        else if (info.type == HitInfo::HIT) {
            // hit, record info
            m_end = c;
        }
        else {
            if (!next_try()) {
                // give up
                kDebug() << "giving up (m_direction =" << m_direction << ")";
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

class DiagonalStrategy : public Strategy
{
    int m_gap;
    int m_offset;
    int m_range;
    
    bool movesAvailable() const {
        Sea::Player opp = Sea::opponent(m_player);
        for (int i = 0; i < m_sea->size().x; i++)
        for (int j = 0; j < m_sea->size().y; j++) {
            if ((j - i - m_offset) % m_gap == 0 &&
                m_sea->at(opp, Coord(i,j)).free()) {
                return true;
            }
        }
        return false;
    }
    
    Coord getMoveHelper()
    {
        int index = rand() % m_range;
        int current = 0;
        for (int y = m_offset; y < m_sea->size().y; y += m_gap) {
            int diag = m_sea->size().y - y;
            if (diag > m_sea->size().x) {
                diag = m_sea->size().x;
            }
            if (index < current + diag) {
                int x = index - current;
                y += x;
                return Coord(x, y);
            }
            current += diag;
        }
        for (int x = m_gap - m_offset; x < m_sea->size().x; x += m_gap) {
            int diag = m_sea->size().x - x;
            if (diag > m_sea->size().y) {
                diag = m_sea->size().y;
            }
            if (index < current + diag) {
                int y = index - current;
                x += y;
                return Coord(x, y);
            }
            current += diag;
        }
        
        return Coord::invalid();
    }
    
    void setup()
    {
        do {
            m_offset = rand() % m_gap;
            kDebug() << "offset =" << m_offset << " / " << m_gap;
        } while (!movesAvailable());
        
        m_range = 0;
        for (int y = m_offset; y < m_sea->size().y; y += m_gap) {
            int diag = m_sea->size().y - y;
            if (diag > m_sea->size().x) {
                diag = m_sea->size().x;
            }
            m_range += diag;
        }
        for (int x = m_gap - m_offset; x < m_sea->size().x; x += m_gap) {
            int diag = m_sea->size().x - x;
            if (diag > m_sea->size().y) {
                diag = m_sea->size().y;
            }
            m_range += diag;
        }
    }
public:
    DiagonalStrategy(Sea::Player player, Sea* sea, SmartAI::State& state, int gap)
    : Strategy(player, sea, state)
    , m_gap(gap)
    {
        setup();
    }

    virtual Coord getMove()
    {
    
        if (!movesAvailable()) {
            kDebug() << "no moves available";
            setup();
        }
        for (int i = 0; i < 50; i++) {
            Coord c = getMoveHelper();
            
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

SmartAI::SmartAI(Sea::Player player, Sea* sea, bool random)
: AI(player, sea)
, m_state(random)
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


SmartAI::State::State(bool random)
: m_random(random)
{
    for (int i = 0; i < LARGEST_SHIP; i++) {
        m_ships[i] = 1;
    }
}

Strategy* SmartAI::State::defaultStrategy(Sea::Player player, Sea* sea)
{
    if (m_random) {
        return new RandomStrategy(player, sea, *this);
    }
    else {
        for (int i = LARGEST_SHIP - 1; i >= 0; i--) {
            if (m_ships[i] > 0 || i == 0) {
                return new DiagonalStrategy(player, sea, *this, i + 1);
            }
        }
    
        // unreachable
        return 0;
    }
}

void SmartAI::State::destroyed(int size)
{
    if (size <= LARGEST_SHIP) {
        int index = size - 1;
        if (m_ships[index] > 0) {
            m_ships[index]--;
        }
    }
}




