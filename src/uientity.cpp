/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "uientity.h"
#include "seaview.h"
#include "shot.h"
#include "ship.h"

#include <QIcon>
#include <QList>

UIEntity::UIEntity(Sea::Player player, Sea* sea, SeaView* view)
: Entity(player, view, sea->battleShipsConfiguration())
, m_sea(sea)
{
}

UIEntity::~UIEntity()
{
    m_seaview->setDelegate(nullptr);
}

void UIEntity::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    drawShoot(Sea::opponent(player), c, info);
}

void UIEntity::notifyGameOver(Sea::Player winner)
{
    drawHiddenShips(winner);
}

void UIEntity::start()
{

}

void UIEntity::hit(Shot* shot)
{
    shot->execute(HitInfo::INVALID);
}

void UIEntity::drawHiddenShips(Sea::Player winner)
{
    const QList<Ship *> enemyShips = m_sea->enemyShips();

    for (Ship * ship : enemyShips) {
        if (ship->alive()) {
            m_seaview->add(winner, ship);
        }
    }
}

void UIEntity::drawShoot(Sea::Player player, const Coord& c, const HitInfo& info)
{
    switch (info.type) {
    case HitInfo::HIT:
        m_seaview->hit(player, c);
//         registerHit(player, c);
        break;
    case HitInfo::MISS:
        m_seaview->miss(player, c);
//         registerMiss(player, c);
        break;
    default:
        return;
    }
    
    if (info.shipDestroyed) {
        Coord shipPos = info.shipPos;
        if (shipPos.valid()) {
            // show destroyed opponent ship
            if (player != m_player) {
                m_seaview->add(player, info.shipDestroyed);
            }

            m_seaview->sink(player, info.shipDestroyed);
        }
    }
}

QIcon UIEntity::icon() const
{
    return QIcon::fromTheme( QLatin1String( "user-identity" ));
}





