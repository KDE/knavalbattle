/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "dummyai.h"
#include "sea.h"
#include <time.h>
#include <QRandomGenerator>

DummyAI::DummyAI(Sea::Player player, Sea* sea, const BattleShipsConfiguration* config)
: AI(player, sea, config)
{
}

Coord DummyAI::getMove()
{
    auto *generator = QRandomGenerator::global();
    if (m_sea->turn() == m_player &&
        m_sea->status() == Sea::PLAYING) {
        for (int i = 0; i < 10000; i++) {
            Coord c(generator->bounded(m_sea->size().x), generator->bounded(m_sea->size().y));
            if (m_sea->canHit(m_player, c)) {
                return c;
            }
        }
    }
    return desperateMove();
}

