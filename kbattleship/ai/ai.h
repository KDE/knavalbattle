#ifndef AI__AI_H
#define AI__AI_H

#include "sea.h"

class AI
{
protected:
    Sea::Player m_player;
    Sea* m_sea;
public:
    AI(Sea::Player player, Sea* sea);
    virtual ~AI() { }
    virtual Coord getMove() = 0;
    virtual void setShips() = 0;
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& hit) = 0;
};

#endif // AI__AI_H

