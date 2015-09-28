/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef SMARTAI_H
#define SMARTAI_H

#include <memory>
#include <QHash>

#include "ai.h"
#include "sea.h"

class Strategy;

class SmartAI : public AI
{
public:
    class State
    {
        QHash<int,int> m_ships;
        bool m_random;
        const BattleShipsConfiguration* m_config;
    public:
        explicit State(bool random, const BattleShipsConfiguration* config);
        Strategy* defaultStrategy(Sea::Player player, Sea*);
        void destroyed(int size);
    };
private:
    std::unique_ptr<Strategy> m_strategy;
    State m_state;
public:
    SmartAI(Sea::Player player, Sea* sea, bool random, const BattleShipsConfiguration* config);

    virtual Coord getMove();
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& hit);
};

#endif // SMARTAI_H
