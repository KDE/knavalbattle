/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "generalcontroller.h"
#include "playerentity.h"
#include "aientity.h"
#include "seaview.h"

GeneralController::GeneralController(QObject* parent)
: QObject(parent)
, m_ready(0)
{
    m_ui = 0;
    m_sea = new Sea(this, Coord(10, 10));
}

void GeneralController::createPlayer(Sea::Player player, SeaView* view)
{
    if (m_ui) {
        kDebug() << "Cannot create more than one human player" << endl;
        exit(1);
    }
    m_ui = new PlayerEntity(player, m_sea, view);
    setupEntity(m_ui);
}

void GeneralController::createAI(Sea::Player player)
{
    Entity* e = new AIEntity(player, m_sea);
    setupEntity(e);
}

void GeneralController::setupEntity(Entity* entity)
{
    entity->setParent(this);
    connect(entity, SIGNAL(shoot(Sea::Player, const Coord&)),
            this, SLOT(shoot(Sea::Player, const Coord&)));
    connect(entity, SIGNAL(ready(Sea::Player)),
            this, SLOT(ready(Sea::Player)));
    m_entities.append(entity);
}

void GeneralController::start(SeaView* view)
{
    if (!m_ui) {
        m_ui = new UIEntity(Sea::NO_PLAYER, m_sea, view);
        setupEntity(m_ui);
    }
    
    foreach (Entity* entity, m_entities) {
        entity->start();
    }
}

void GeneralController::shoot(Sea::Player player, const Coord& c)
{
    Entity* entity = findEntity(Sea::opponent(player));
    if (!entity) {
        return;
    }

    HitInfo info = entity->hit(player, c);
    
    if (info.type != HitInfo::INVALID) {
        // notify entities
        notify(player, c, info);
            
        if (m_sea->status() == Sea::A_WINS) {
            emit gameOver(Sea::PLAYER_A);
        }
        else if (m_sea->status() == Sea::B_WINS) {
            emit gameOver(Sea::PLAYER_B);
        }
    }
    else {
        kDebug() << "illegal move " << c << " for player " << player << endl;
    }
}

void GeneralController::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    foreach (Entity* entity, m_entities) {
        entity->notify(player, c, info);
    }
}

void GeneralController::ready(Sea::Player)
{
    m_ready++;
    kDebug() << "ready = " << m_ready << endl;
    if (m_ready >= 2) {
        m_sea->startPlaying();
        foreach (Entity* entity, m_entities) {
            entity->startPlaying();
        }
    }
}

const Stats* GeneralController::stats() const
{
    foreach (Entity* entity, m_entities) {
        const Stats* res = entity->stats();
        if (res) {
            return res;
        }
    }
    
    return 0;
}

Entity* GeneralController::findEntity(Sea::Player player) const
{
    foreach (Entity* entity, m_entities) {
        if (entity->player() == player) {
            return entity;
        }
    }
    
    return 0;
}

#include "generalcontroller.moc"

