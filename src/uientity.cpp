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

#include <KIcon>

UIEntity::UIEntity(Sea::Player player, Sea* sea, SeaView* view)
: Entity(player)
, m_sea(sea)
, m_view(view)
{
}

UIEntity::~UIEntity()
{
    m_view->setDelegate(0);
}

void UIEntity::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    drawShoot(Sea::opponent(player), c, info);
}

void UIEntity::start()
{
    m_view->clear();
}

void UIEntity::hit(Shot* shot)
{
    shot->execute(HitInfo::INVALID);
}

void UIEntity::drawShoot(Sea::Player player, const Coord& c, const HitInfo& info)
{
    switch (info.type) {
    case HitInfo::HIT:
        m_view->hit(player, c);
//         registerHit(player, c);
        break;
    case HitInfo::MISS:
        m_view->miss(player, c);
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
                m_view->add(player, shipPos, info.shipDestroyed);
            }
            
            m_view->sink(player, shipPos, info.shipDestroyed);
        }
    }
}

KIcon UIEntity::icon() const
{
    return KIcon("user-female");
}

#include "uientity.moc"



