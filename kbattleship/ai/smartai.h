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

#include "ai.h"
#include "sea.h"

class Strategy;

class SmartAI : public AI
{
public:
    class State
    {
        static const int LARGEST_SHIP = 4;
        int m_ships[LARGEST_SHIP];
    public:
        State();
        Strategy* defaultStrategy(Sea::Player player, Sea*);
        void destroyed(int size);
    };
private:
    std::auto_ptr<Strategy> m_strategy;
    State m_state;
public:
    SmartAI(Sea::Player player, Sea* sea);
    
    
    virtual Coord getMove();
    virtual void setShips();
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& hit);
};

#endif // SMARTAI_H
