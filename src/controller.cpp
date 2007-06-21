/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "controller.h"

#include <klocalizedstring.h>
#include "playerentity.h"
#include "aientity.h"
#include "networkentity.h"
#include "seaview.h"
#include "shot.h"
#include "audioplayer.h"

Controller::Controller(QObject* parent, AudioPlayer* player)
: QObject(parent)
, m_shot(0)
, m_ready(0)
, m_player(player)
{
    m_ui = 0;
    m_sea = new Sea(this, Coord(10, 10));
}

PlayerEntity* Controller::createPlayer(Sea::Player player, SeaView* view,
                                              ChatWidget* chat, const QString& nick)
{
    if (m_ui) {
        kDebug() << "Cannot create more than one human player" << endl;
        return 0;
    }
    PlayerEntity* entity = new PlayerEntity(player, m_sea, view, chat);
    entity->setNick(nick);
    m_ui = entity;
    setupEntity(m_ui);
    return entity;
}

void Controller::createAI(Sea::Player player)
{
    Entity* e = new AIEntity(player, m_sea);
    e->setNick(i18n("Computer"));
    setupEntity(e);
}

void Controller::createRemotePlayer(Sea::Player player, QIODevice* device, bool client)
{
    Entity* e = new NetworkEntity(player, m_sea, device, client);
    setupEntity(e);
    if (client) {
        m_sea->switchTurn();
    }
}

void Controller::setupEntity(Entity* entity)
{
    entity->setParent(this);
    connect(entity, SIGNAL(shoot(int, const Coord&)),
            this, SLOT(shoot(int, const Coord&)), Qt::QueuedConnection);
    connect(entity, SIGNAL(ready(int)),
            this, SLOT(ready(int)), Qt::QueuedConnection);
    connect(entity, SIGNAL(chat(QString)),
            this, SLOT(receivedChat(QString)));
    connect(entity, SIGNAL(nick(int,QString)),
            this, SLOT(nick(int,QString)));
            
    foreach (Entity* e, m_entities) {
        connect(e, SIGNAL(compatibility(int)),
                entity, SLOT(setCompatibilityLevel(int)));
        connect(entity, SIGNAL(compatibility(int)),
                e, SLOT(setCompatibilityLevel(int)));
    }
            
    m_entities.append(entity);
}

bool Controller::allPlayers() const
{
    unsigned char bitmap = 0;
    foreach (Entity* entity, m_entities) {
        int player = entity->player();
        kDebug() << "found player " << player << endl;
        bitmap |= (1 << player);    
    }
    
    kDebug() << "bitmap = " << (unsigned) bitmap << endl;
    return bitmap == 3;
}

bool Controller::start(SeaView* view)
{
    if (!allPlayers()) {
        return false;
    }
    
    if (!m_ui) {
        m_ui = new UIEntity(Sea::NO_PLAYER, m_sea, view);
        setupEntity(m_ui);
    }
    
    foreach (Entity* entity, m_entities) {
        entity->start();
    }
    
    foreach (Entity* source, m_entities) {
        foreach (Entity* target, m_entities) {
            if (source->player() != target->player() &&
                !source->nick().isEmpty()) {
                target->notifyNick(source->player(), source->nick());
            }
        }
    }
    return true;
}

void Controller::shoot(int player, const Coord& c)
{
    Entity* entity = findEntity(Sea::opponent(Sea::Player(player)));
    if (!entity) {
        kDebug() << "no entity!" << endl;
        return;
    }

    if (m_shot) {
        kDebug() << "shot in progress" << endl;
        // shot in progress
        return;
    }
    
    if (m_sea->status() == Sea::PLAYING) {
        entity->hit(m_shot = new Shot(this, Sea::Player(player), c)); // kind of CPS
    }
}

void Controller::finalizeShot(Sea::Player player, const Coord& c, const HitInfo& info)
{
    if (info.type != HitInfo::INVALID) {
        // notify entities
        notify(player, c, info);
        
        // play sounds
        if (m_player) {
            m_player->play(player, info);
        }
            
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
    
    delete m_shot;
    m_shot = 0;
}

void Controller::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    foreach (Entity* entity, m_entities) {
        entity->notify(player, c, info);
    }
}

void Controller::ready(int player)
{
    m_ready++;
    foreach (Entity* entity, m_entities) {
        entity->notifyReady(Sea::Player(player));
    }
    
    // when two entities are ready, start
    // all engines
    if (m_ready >= 2) {
        m_sea->startPlaying();
        foreach (Entity* entity, m_entities) {
            entity->startPlaying();
        }
    }
}

const Stats* Controller::stats() const
{
    foreach (Entity* entity, m_entities) {
        const Stats* res = entity->stats();
        if (res) {
            return res;
        }
    }
    
    return 0;
}

Entity* Controller::findEntity(Sea::Player player) const
{
    foreach (Entity* entity, m_entities) {
        if (entity->player() == player) {
            return entity;
        }
    }
    
    return 0;
}

void Controller::receivedChat(const QString& text)
{
    Entity* chat_sender = qobject_cast<Entity*>(sender());
    
    if (chat_sender) {    
        foreach (Entity* entity, m_entities) {
            if (entity != chat_sender) {
                kDebug() << "forwarding to " << entity->nick() << endl;
                entity->notifyChat(chat_sender, text);
            }
        }
    }
}

void Controller::nick(int player, const QString& nick)
{
    kDebug() << "controller: nick" << endl;
    foreach (Entity* entity, m_entities) {
        if (entity->player() != Sea::Player(player)) {
            entity->notifyNick(Sea::Player(player), nick);
        }
    }
}

#include "controller.moc"


