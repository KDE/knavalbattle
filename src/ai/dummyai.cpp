/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "dummyai.h"
#include "sea.h"

// for rand
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

DummyAI::DummyAI(Sea::Player player, Sea* sea, const BattleShipsConfiguration* config)
: AI(player, sea, config)
{
    srand(time(0));
}

Coord DummyAI::getMove()
{
    if (m_sea->turn() == m_player &&
        m_sea->status() == Sea::PLAYING) {
        for (int i = 0; i < 10000; i++) {
            Coord c(rand() % m_sea->size().x, rand() % m_sea->size().y);
            if (m_sea->canHit(m_player, c)) {
                return c;
            }
        }
    }
    return desperateMove();
}

