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

DummyAI::DummyAI(Sea::Player player, Sea* sea)
: AI(player, sea)
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

void DummyAI::setShips()
{
    // set up computer ships
    for (int i = 1; i <= 4; i++) {
        Ship* ship = 0;
        while (ship == 0) {
            Coord c(rand() % m_sea->size().x, rand() % m_sea->size().y);
            Ship::Direction dir = rand() % 2 == 0 ? Ship::LEFT_TO_RIGHT : Ship::TOP_DOWN;
            if (m_sea->canAddShip(m_player, c, i, dir)) {
                ship = new Ship(i, dir);
                m_sea->add(m_player, c, ship);
            }
        }
    }
}

