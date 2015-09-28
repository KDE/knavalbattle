/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "controller.h"

#include <KLocalizedString>

#include "playerentity.h"
#include "aientity.h"
#include "networkentity.h"
#include "seaview.h"
#include "shot.h"
#include "audioplayer.h"
#include "ships.h"

Controller::Controller(QObject* parent, AudioPlayer* audioPlayer, const BattleShipsConfiguration& battleShipsConfiguration)
: QObject(parent)
, m_shot(0)
, m_ready(0)
, m_player(audioPlayer)
, m_has_ai(false)
, mBattleShipsConfiguration(battleShipsConfiguration)
{
    m_ui = 0;
    m_sea = new Sea(this, battleShipsConfiguration);
}

PlayerEntity* Controller::createPlayer(Sea::Player player, SeaView* view,
                                              ChatWidget* chat, const QString& nick)
{
    if (m_ui) {
        qDebug() << "Cannot create more than one human player";
        return 0;
    }
    PlayerEntity* entity = new PlayerEntity(player, m_sea, view, chat);
    entity->setNick(nick);
    m_ui = entity;
    setupEntity(m_ui);
    return entity;
}

AIEntity* Controller::createAI(Sea::Player player, SeaView* view)
{
    qDebug() << "created ai entity";
    m_has_ai = true;
    AIEntity* e = new AIEntity(player, m_sea, view);
    e->setNick(i18n("Computer"));
    setupEntity(e);

    return e;
}

NetworkEntity* Controller::createRemotePlayer(Sea::Player player, SeaView* view, Protocol* protocol, bool client)
{
    NetworkEntity* e = new NetworkEntity(player, m_sea, view, protocol, client);
    setupEntity(e);
    connect(e, &NetworkEntity::restartRequested, this, &Controller::restartRequested);
    if (client) {
        m_sea->switchTurn();
    }
    return e;
}

void Controller::setupEntity(Entity* entity)
{
    entity->setParent(this);

    connect(entity, &Entity::shoot,
            this, &Controller::shoot, Qt::QueuedConnection);
    connect(entity, &Entity::ready,
            this, &Controller::ready);
    connect(entity, &Entity::shipsPlaced,
            this, &Controller::shipsPlaced);
    connect(entity, &Entity::chat,
            this, &Controller::receivedChat);
    connect(entity, &Entity::nickChanged,
            this, &Controller::nick);
    connect(entity, &Entity::compatibility,
            this, &Controller::compatibility);
    connect(entity, &Entity::gameOptionsInterchanged,
            this, &Controller::placing);

    foreach (Entity* e, m_entities) {
        connect(e, &Entity::compatibility,
                entity, &Entity::setCompatibilityLevel);
        connect(entity, &Entity::compatibility,
                e, &Entity::setCompatibilityLevel);

        connect(e, &Entity::abortGame,
                entity, &Entity::notifyAbort);
        connect(entity, &Entity::abortGame,
                e, &Entity::notifyAbort);
        connect(e, &Entity::restartPlacingShips,
                this, &Controller::restartPlacingShips);
        connect(e, &Entity::restartPlacingShips,
                this, &Controller::notifyRestartPlacingShips);
    }

    m_entities.append(entity);
}

void Controller::setBattleShipsConfiguration(const BattleShipsConfiguration& battleConfiguration)
{
    mBattleShipsConfiguration = battleConfiguration;
}


bool Controller::allPlayers() const
{
    unsigned char bitmap = 0;
    foreach (Entity* entity, m_entities) {
        int player = entity->player();
        qDebug() << "found player" << player;
        bitmap |= (1 << player);    
    }
    
    qDebug() << "bitmap =" << (unsigned) bitmap;
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
        entity->notifyGameOptions();
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

void Controller::restart(bool ask)
{
    m_ready = 0;
    if (ask)
    {
        foreach (Entity* entity, m_entities) {
            entity->notifyRestart(entity->player());
        }
    }

    m_sea->clear(Sea::PLAYER_A);
    m_sea->clear(Sea::PLAYER_B);

    foreach (Entity* entity, m_entities) {
        m_sea->clear(entity->player());
            emit startPlacingShips(Sea::PLAYER_A);
            entity->startPlacing();
    }
}


// It is sure the entities has interchanged the GameOptions (if any)
// when the opposite nick is received
void Controller::placing()
{
    foreach (Entity* entity, m_entities) {
        entity->startPlacing();
    }
}

void Controller::shoot(int player, const Coord& c)
{
    Entity* entity = findEntity(Sea::opponent(Sea::Player(player)));
    if (!entity) {
        qDebug() << "no entity!";
        return;
    }

    if (m_shot) {
        qDebug() << "shot in progress";
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
            finalizeGame(Sea::PLAYER_A);
        }
        else if (m_sea->status() == Sea::B_WINS) {
            finalizeGame(Sea::PLAYER_B);
        }
        else {
            emit turnChanged(m_sea->turn());
        }
    }
    else {
        qDebug() << "illegal move" << c << "for player" << player;
    }
    
    delete m_shot;
    m_shot = 0;
}

void Controller::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    foreach (Entity* entity, m_entities) {
        entity->notify(player, c, info);
        if (player == entity->player()) {
            entity->stats()->addInfo(info);
        }
    }
}

void Controller::shipsPlaced(int player)
{
    m_ready++;
    if (m_ready >= 2 )
    {
        foreach (Entity* entity, m_entities) {
            entity->start();
        }
    }
}


void Controller::ready(int player)
{
    m_ready++;
    foreach (Entity* entity, m_entities) {
        entity->notifyReady(Sea::Player(player));
    }
    // when two entities are ready (ships placed and ready)
    // start all engines
    if (m_ready >= 4) {
        m_sea->startPlaying();

        foreach (Entity* entity, m_entities) {
            entity->startPlaying();
        }
        emit playerReady(-1);
    }
    else {
        emit playerReady(player);
    }
}

void Controller::finalizeGame(Sea::Player winner)
{
    // first, every entity will notify the other entity its ships
    foreach (Entity* entity, m_entities) {
            entity->notifyShips(winner);
    }
    // then, it will notify the end of the game
    foreach (Entity* entity, m_entities) {
        entity->notifyGameOver(winner);
    }
    emit gameOver(winner);
}

void Controller::notifyRestartPlacingShips(Sea::Player player)
{
    foreach (Entity* entity, m_entities) {
        if (entity->player() == player) {
            entity->notifyRestartPlacing(player);
        }
    }
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
                qDebug() << "forwarding to" << entity->nick();
                entity->notifyChat(chat_sender, text);
            }
        }
    }
}

void Controller::nick(int player, const QString& nick)
{
    qDebug() << "controller: nick";
    foreach (Entity* entity, m_entities) {
        if (entity->player() != Sea::Player(player)) {
            entity->notifyNick(Sea::Player(player), nick);
        }
    }
    emit nickChanged(player, nick);
}

Sea::Player Controller::turn() const
{
    return m_sea->turn();
}

bool Controller::hasAI() const
{
    return m_has_ai;
}


