/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI__AI_H
#define AI__AI_H

#include "sea.h"
#include "ships.h"

class AI
{
protected:
    Sea::Player m_player;
    Sea* m_sea;
    const BattleShipsConfiguration* m_config;
public:
    AI(Sea::Player player, Sea* sea, const BattleShipsConfiguration* config);
    virtual ~AI() { }
    virtual Coord getMove() = 0;
    virtual void setShips();
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& hit) = 0;
    Coord desperateMove() const;
};

#endif // AI__AI_H

