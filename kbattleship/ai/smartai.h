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
        int m_largest;
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
