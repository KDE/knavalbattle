#ifndef AI_DUMMYAI_H
#define AI_DUMMYAI_H

#include "ai.h"

class DummyAI : public AI
{
public:
    DummyAI(Sea::Player player, Sea* sea);
    virtual Coord getMove();
    virtual void setShips();
};

#endif // AI_DUMMYAI_H

