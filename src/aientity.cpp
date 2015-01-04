/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "aientity.h"
#include "ai/smartai.h"
#include "ai/dummyai.h"
#include "shot.h"
#include "seaview.h"

#include <KgDifficulty>

#include <QIcon>

AIEntity::AIEntity(Sea::Player player, Sea* sea, SeaView *seaview)
: Entity(player, seaview, sea->battleShipsConfiguration())
, m_sea(sea)
{
    switch (Kg::difficultyLevel()) {
    case KgDifficultyLevel::Easy:
        m_ai = new DummyAI(m_player, m_sea, sea->battleShipsConfiguration());
        break;
    case KgDifficultyLevel::Medium:
        m_ai = new SmartAI(m_player, m_sea, true, sea->battleShipsConfiguration());
        break;
    default: // hard
        m_ai = new SmartAI(m_player, m_sea, false, sea->battleShipsConfiguration());
        break;
    }
}

AIEntity::~AIEntity()
{
    delete m_ai;
}

void AIEntity::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    m_ai->notify(player, c, info);

    // add a border around opponent sunk ship in KBS4 mode when no adjacentShips
    // are allowed, the AI will not try to shoot at points that are not free
    if (m_level == COMPAT_KBS4 &&
        player == m_player &&
        !m_sea->isAdjacentShipsAllowed() &&
        info.shipDestroyed) {
            m_sea->addBorder(Sea::opponent(player), info.shipPos);
    }

    getShoot();
}

void AIEntity::notifyGameOptions(bool ask)
{
    emit gameOptionsInterchanged();
}

void AIEntity::start(bool)
{
    emit ready(m_player);
}

void AIEntity::startPlacing(bool ask)
{
    m_seaview->setStatus(Sea::PLACING_SHIPS);
    m_ai->setShips();
    emit shipsPlaced(m_player);
}

void AIEntity::startPlaying()
{
    getShoot();
    m_seaview->setStatus(Sea::PLAYING);
}

void AIEntity::hit(Shot* shot)
{
    if (shot->player() != m_player && m_sea->canHit(shot->player(), shot->pos())) {
        shot->execute(m_sea->hit(shot->pos()));
    }
    else {
        shot->execute(HitInfo::INVALID);
    }
}

void AIEntity::getShoot()
{
    if (m_sea->turn() == m_player) {
        Coord c = m_ai->getMove();
        
        emit shoot(m_player, c);
    }
}

QIcon AIEntity::icon() const
{
    return QIcon::fromTheme( QLatin1String( "roll" ));
}




