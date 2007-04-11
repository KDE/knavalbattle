#ifndef SHOT_H
#define SHOT_H

#include "sea.h"

class GeneralController;

class Shot
{
    GeneralController* m_parent;
    Sea::Player m_player;
    Coord m_pos;
public:
    Shot(GeneralController* parent, Sea::Player player, const Coord& pos);
    void execute(const HitInfo& info);
    
    Sea::Player player() const { return m_player; }
    const Coord& pos() const { return m_pos; }
};


#endif // SHOT_H
