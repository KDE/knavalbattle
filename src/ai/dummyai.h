/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef AI_DUMMYAI_H
#define AI_DUMMYAI_H

#include "ai.h"

class DummyAI : public AI
{
public:
    DummyAI(Sea::Player player, Sea* sea, const BattleShipsConfiguration* config);
    Coord getMove() override;
    void notify(Sea::Player, const Coord&, const HitInfo&) override { }
};

#endif // AI_DUMMYAI_H

