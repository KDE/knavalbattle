/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "playerentity.h"

#include <KMessageBox>
#include <KLocalizedString>

#include "seaview.h"
#include "shot.h"
#include "coord.h"
#include "chatwidget.h"

PlayerEntity::PlayerEntity(Sea::Player player, Sea* sea, SeaView* view, ChatWidget* chat)
: UIEntity(player, sea, view)
, m_chat(chat)
{
    m_seaview->setStatus(Sea::PLACING_SHIPS);
}

Ship* PlayerEntity::nextShip()
{
    if (m_ships.empty()) {
        return 0;
    }
    else {
        return m_ships.first();
    }
}

Ship* PlayerEntity::canAddShip(const Coord& c)
{
    Ship* ship = nextShip();
    if (ship) {
        ship->setPosition(c);
        if (m_sea->canAddShip(m_player, c, ship->size(), ship->direction())) {
            // check if it is near any other ship
            // in KBS3 mode
            if (m_level == COMPAT_KBS3) {
                for (unsigned int i = 0; i < ship->size(); i++) {
                    if (m_sea->isNearShip(m_player, c + ship->increment() * i)) {
                        return 0;
                    }
                }
            }
            return ship;
        }
    }
    return 0;
}


void PlayerEntity::action(Sea::Player player, const Coord& c)
{
    Sea::Player opponent = Sea::opponent(m_player);
    switch ( m_sea->status() )
    {
        case Sea::PLACING_SHIPS: 
            if (!m_ships.empty()) {
                if (player == m_player) {
                    // placing ships
                    // First check if the ship can be placed anywhere
                    Ship* ship = canAddShip(c);
                    if (ship) {
                        // remove ship from the list
                        m_ships.removeFirst();

                        // add ship to the sea
                        m_sea->add(m_player, ship);
                        m_seaview->add(m_player, ship);
                        if (!m_ships.empty()) {
                            ship=m_ships.first();
                            // when multiple ships and an space between them are enabled,
                            // it is possible to reach impossible combinations
                            if ( !m_sea->canAddShipOfSize(m_player, ship->size()) ) {
                                emit restartPlacingShips(m_player);
                            }
                        }
                        else {
                            emit shipsPlaced();
                        }
                    }
                }
            }
            break;
        case Sea::PLAYING:
            if (player == opponent && m_sea->canHit(m_player, c)) {
                emit shoot(m_player, c);
            }
            break;
        default:
            // SHOULD NEVER HAPPEN
            break;
    }
}

void PlayerEntity::startPlacing()
{
    UIEntity::startPlacing();
    m_battleShipsConfiguration = m_sea->battleShipsConfiguration();

    Coord origin(0, 0);

    m_sea->clear(m_player);
    qDeleteAll(m_ships);
    m_ships.clear();
    for (unsigned int len=1; len <= m_battleShipsConfiguration->longestShip(); len++)
    {
        for (unsigned int i=0; i<m_battleShipsConfiguration->numberOfShipsOfSize(len); i++)
        {
            m_ships.append(new Ship(len, Ship::LEFT_TO_RIGHT, origin));
        }
    }
    m_seaview->setDelegate(this);
    m_seaview->setStatus(Sea::PLACING_SHIPS);
}


void PlayerEntity::start()
{
    UIEntity::start();
    emit ready(m_player);
}

void PlayerEntity::startPlaying()
{
    UIEntity::startPlaying();
    m_seaview->setStatus(Sea::PLAYING);
}

void PlayerEntity::hit(Shot* shot)
{
    if (shot->player() != m_player && m_sea->canHit(shot->player(), shot->pos())) {
        HitInfo info = m_sea->hit(shot->pos());
        shot->execute(info);
    }
    else {
        shot->execute(HitInfo::INVALID);
    }
}

void PlayerEntity::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    UIEntity::notify(player, c, info);
}

void PlayerEntity::changeDirection(Sea::Player player)
{
    if (player == m_player) {
        Ship* next = nextShip();
        if (next) {
            next->changeDirection();
            m_seaview->cancelPreview();
        }
    }
}

bool PlayerEntity::canAddShip(Sea::Player player, const Coord& c)
{
    if (m_ships.isEmpty() || player != m_player) {
        return false;
    }

    Ship * next = m_ships.at(0);

    if (m_sea->canAddShip(player, c, next->size(), next->direction())) {
        return true;
    }

    return false;
}

void PlayerEntity::registerHit(Sea::Player player, const Coord&)
{
    if (player == m_player) {
        m_stats.addHit();
    }
}

void PlayerEntity::registerMiss(Sea::Player player, const Coord&)
{
    if (player == m_player) {
        m_stats.addMiss();
    }
}

void PlayerEntity::notifyChat(const Entity* entity, const QString& text)
{
    if (entity != this) {
        qCDebug(KNAVALBATTLE_LOG) << "received chat from" << entity->nick() << ":" << text;
        m_chat->display(entity->nick(), text);
    }
}

void PlayerEntity::notifyNick(Sea::Player, const QString& nickname)
{
    m_chat->display(i18n("Your opponent is now known as %1", nickname));
}

void PlayerEntity::notifyAbort()
{
    m_chat->display(i18n("Your opponent disconnected from the game"));
}

void PlayerEntity::setNick(const QString& nick)
{
    UIEntity::setNick(nick);
    m_chat->setNick(nick);
}

void PlayerEntity::notifyRestartPlacing(Sea::Player player)
{
    UIEntity::notifyRestartPlacing(player);
    m_seaview->clear();
    m_sea->clear(player);
    startPlacing();
}





