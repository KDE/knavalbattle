/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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

    Coord getMove() override;
    void notify(Sea::Player player, const Coord& c, const HitInfo& hit) override;
};

#endif // SMARTAI_H
