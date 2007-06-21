/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "playerentity.h"

#include <kmessagebox.h>
#include <klocale.h>
#include "seaview.h"
#include "shot.h"
#include "chatwidget.h"

PlayerEntity::PlayerEntity(Sea::Player player, Sea* sea, SeaView* view, ChatWidget* chat)
: UIEntity(player, sea, view)
, m_chat(chat)
{
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
    Q_ASSERT(ship);
    
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
    
    return 0;
}

void PlayerEntity::action(Sea::Player player, const Coord& c)
{
    if (nextShip()) {
        if (player == m_player) {
            // placing ships
            Ship* ship = canAddShip(c);
            if (ship) {
                // remove ship from the list
                m_ships.removeFirst();
                
                // add ship to the sea
                m_sea->add(m_player, c, ship);
                m_view->add(m_player, c, ship);
                
                if (!nextShip()) {
                    emit ready(m_player);
                }
            }
        }
    }
    else {
        Sea::Player opponent = Sea::opponent(m_player);
        if (player == opponent && m_sea->canHit(m_player, c)) {
            emit shoot(m_player, c);
        }
    }
}

void PlayerEntity::start()
{
    m_ships.append(new Ship(1, Ship::LEFT_TO_RIGHT));
    m_ships.append(new Ship(2, Ship::LEFT_TO_RIGHT));
    m_ships.append(new Ship(3, Ship::LEFT_TO_RIGHT));
    m_ships.append(new Ship(4, Ship::LEFT_TO_RIGHT));
    
    UIEntity::start();
    m_view->setDelegate(this);
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
    
    // add a border around sunk ship in KBS3 mode
    if (m_level == COMPAT_KBS3 &&
        player == m_player && 
        info.shipDestroyed) {
        m_sea->addBorder(player, info.shipPos);
    }
}

void PlayerEntity::changeDirection(Sea::Player player)
{
    if (player == m_player) {
        Ship* next = nextShip();
        if (next) {
            next->changeDirection();
            m_view->cancelPreview();
        }
    }
}

Ship* PlayerEntity::canAddShip(Sea::Player player, const Coord& c)
{
    Ship* next = nextShip();
    if (next == 0 || player != m_player) {
        return 0;
    }
    if (m_sea->canAddShip(player, c, next->size(), next->direction())) {
        return next;
    }
    else {
        return 0;
    }
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

void PlayerEntity::notifyChat(const QString& nickname, const QString& text)
{
    kDebug() << "received chat from " << nickname << ": " << text << endl;
    m_chat->display(nickname, text);
}

void PlayerEntity::notifyNick(Sea::Player, const QString& nickname)
{
    m_chat->display(i18n("Your opponent is now known as %1", nickname));
}

void PlayerEntity::setNick(const QString& nick)
{
    UIEntity::setNick(nick);
    m_chat->setNick(nick);
}

void PlayerEntity::setCompatibilityLevel(int level)
{
    UIEntity::setCompatibilityLevel(level);
    KMessageBox::information(m_view, i18n("Your opponent is using pre-KDE4 version of KBattleship. Note that, according to the rules enforced by old clients, ships cannot be placed adjacent to one another."));
}

