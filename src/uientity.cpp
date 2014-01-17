/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "uientity.h"
#include "seaview.h"
#include "shot.h"
#include "ship.h"

#include <KIcon>
#include <QList>

UIEntity::UIEntity(Sea::Player player, Sea* sea, SeaView* view)
: Entity(player, view, sea->battleShipsConfiguration())
, m_sea(sea)
{
}

UIEntity::~UIEntity()
{
    m_seaview->setDelegate(0);
}

void UIEntity::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    drawShoot(Sea::opponent(player), c, info);
}

void UIEntity::notifyGameOver(Sea::Player winner)
{
    drawHiddenShips(winner);
}

void UIEntity::start(bool)
{

}

void UIEntity::hit(Shot* shot)
{
    shot->execute(HitInfo::INVALID);
}

void UIEntity::drawHiddenShips(Sea::Player winner)
{
    QList<Ship *> enemyShips = m_sea->enemyShips();

    foreach (Ship * ship, enemyShips) {
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

KIcon UIEntity::icon() const
{
    return KIcon( QLatin1String( "user-identity" ));
}

#include "uientity.moc"



