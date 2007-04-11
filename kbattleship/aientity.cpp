/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "aientity.h"
#include "ai/smartai.h"

AIEntity::AIEntity(Sea::Player player, Sea* sea)
: Entity(player)
, m_sea(sea)
{
    m_ai = new SmartAI(m_player, m_sea);
}

AIEntity::~AIEntity()
{
    delete m_ai;
}

void AIEntity::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    m_ai->notify(player, c, info);
    getShoot();
}

void AIEntity::start()
{
    kDebug() << m_player << ": starting" << endl;
    m_ai->setShips();
    emit ready(m_player);
}

void AIEntity::startPlaying()
{
    getShoot();
}

void AIEntity::getShoot()
{
    if (m_sea->turn() == m_player) {
        kDebug() << "status = " << m_sea->status() << endl;
        Coord c = m_ai->getMove();
        kDebug() << m_player << ": ai returned " << c << endl;
        if (m_sea->canHit(m_player, c)) {
            HitInfo info = m_sea->hit(c);
            kDebug() << m_player << ": shooting " << c << endl;
            emit shoot(m_player, c, info);
            m_ai->notify(m_player, c, info);
        }
    }
}

#include "aientity.moc"


