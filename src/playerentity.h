/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include <QList>

#include "sea.h"
#include "uientity.h"
#include "delegate.h"
#include "stats.h"

class ChatWidget;

class PlayerEntity : public UIEntity, private Delegate
{
Q_OBJECT
    QList<Ship*> m_ships;
    
    Ship* canAddShip(const Coord& c);
    
    ChatWidget* m_chat;
public:
    PlayerEntity(Sea::Player player, Sea* sea, SeaView* view, ChatWidget* chat);
    
    // entity interface
    void start() override;
    void startPlacing() override;
    void startPlaying() override;
    void hit(Shot* shot) override;
    void notify(Sea::Player player, const Coord& c, const HitInfo& info) override;
    void notifyChat(const Entity* entity, const QString& text) override;
    void notifyNick(Sea::Player player, const QString& text) override;
    void notifyGameOptions() override { };
    // delegate interface
    void action(Sea::Player player, const Coord& c) override;
    void changeDirection(Sea::Player player) override;
    bool canAddShip(Sea::Player player, const Coord& c) override;
    Ship * nextShip() override;
    
    void setNick(const QString& nick) override;
protected:
    // parent interface
    void registerHit(Sea::Player player, const Coord& c) override;
    void registerMiss(Sea::Player player, const Coord& c) override;
public Q_SLOTS:
    void notifyAbort() override;
    void notifyRestartPlacing(Sea::Player player) override;
};

#endif // PLAYERENTITY_H
