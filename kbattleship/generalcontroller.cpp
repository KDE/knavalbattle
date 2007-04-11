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
    m_entities[0] = 0;
    m_entities[1] = 0;
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
    m_entities[player] = m_ui;
    setupEntity(m_entities[player]);
}

void GeneralController::createAI(Sea::Player player)
{
    m_entities[player] = new AIEntity(player, m_sea);
    setupEntity(m_entities[player]);
}

void GeneralController::setupEntity(Entity* entity)
{
    entity->setParent(this);
    connect(entity, SIGNAL(shoot(Sea::Player, const Coord&, const HitInfo&)),
            this, SLOT(shoot(Sea::Player, const Coord&, const HitInfo&)));
    connect(entity, SIGNAL(ready(Sea::Player)),
            this, SLOT(ready(Sea::Player)));
}

void GeneralController::start(SeaView* view)
{
    for (int i = 0; i < 2; i++) {
        if (!m_entities[i]) {
            createAI(Sea::Player(i));
        }
    }
    if (!m_ui) {
        m_ui = new UIEntity(Sea::NO_PLAYER, m_sea, view);
        setupEntity(m_ui);
    }
    
    m_entities[0]->start();
    m_entities[1]->start();
    if (!m_ui->started()) {
        m_ui->start();
    }
}

void GeneralController::shoot(Sea::Player player, const Coord& c, const HitInfo& info)
{
    // move validation is assumed to have happened in the generating Entity
    // notify the opponent
    notify(Sea::opponent(player), player, c, info);
    
    if (m_sea->status() == Sea::A_WINS) {
        emit gameOver(Sea::PLAYER_A);
    }
    else if (m_sea->status() == Sea::B_WINS) {
        emit gameOver(Sea::PLAYER_B);
    }
}

void GeneralController::notify(Sea::Player target, Sea::Player player, const Coord& c, const HitInfo& info)
{
    if (m_entities[target]) {
        m_entities[target]->notify(player, c, info);
    }
}

void GeneralController::ready(Sea::Player)
{
    m_ready++;
    kDebug() << "ready = " << m_ready << endl;
    if (m_ready >= 2) {
        m_sea->startPlaying();
        m_entities[0]->startPlaying();
        m_entities[1]->startPlaying();
    }
}

const Stats* GeneralController::stats() const
{
    const Stats* res = 0;
    if (m_entities[0]) {
        res = m_entities[0]->stats();
    }
    if (res) {
        return res;
    }
    if (m_entities[1]) {
        res = m_entities[1]->stats();
    }
    return res;
}

#include "generalcontroller.moc"


