/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ai.h"
#include <QRandomGenerator>

AI::AI(Sea::Player player, Sea* sea, const BattleShipsConfiguration* config)
: m_player(player)
, m_sea(sea)
, m_config(config)
{
}

Coord AI::desperateMove() const
{
    Sea::Player opp = Sea::opponent(m_player);
    for (int i = 0; i < m_sea->size().x; i++)
    for (int j = 0; j < m_sea->size().y; j++) {
        if (m_sea->at(opp, Coord(i,j)).free()) {
            return Coord(i,j);
        }
    }
    return Coord::invalid();
}

void AI::setShips()
{
    // set up computer ships
    // set first the biggest ship, it is more difficult to reach impossible combinations
    // TODO: Another placing algorithm, create a list of available places and choose randomly from them.
    // number of repetitions because the random place is over a previous ship = 0
    bool canFinish = true;
    auto *generator = QRandomGenerator::global();
    do {
        for (int size = m_config->longestShip(); size >= 1; size--) {
            for (unsigned int j = 1; j <= m_config->numberOfShipsOfSize(size); j++) {
                Ship* ship = nullptr;
                while (ship == nullptr && canFinish) {
                    Coord c(generator->bounded(m_sea->size().x), generator->bounded(m_sea->size().y));
                    Ship::Direction dir = generator->bounded(2) == 0 ? Ship::LEFT_TO_RIGHT : Ship::TOP_DOWN;
                    if (m_sea->canAddShip(m_player, c, size, dir)) {
                        ship = new Ship(size, dir, c);
                        m_sea->add(m_player, ship);
                    }
                    else {
                        canFinish = m_sea->canAddShipOfSize(m_player, size);
                    }
                }
            }
        }
    } while ( !canFinish );
}
