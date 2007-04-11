/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "aientity.h"
#include "ai/smartai.h"

#include <QTimer>

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
        Coord c = m_ai->getMove();
        
        QTimer::singleShot(100, new DelayedShot(this, m_player, c), SLOT(shoot()));
//         emit shoot(m_player, c);
    }
}

DelayedShot::DelayedShot(AIEntity* parent, Sea::Player player, const Coord& pos)
: QObject(parent)
, m_parent(parent)
, m_player(player)
, m_pos(pos)
{
}

void DelayedShot::shoot()
{
    emit m_parent->shoot(m_player, m_pos);
    deleteLater();
}

#include "aientity.moc"


