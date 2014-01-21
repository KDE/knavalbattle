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
    virtual void start(bool);
    virtual void startPlacing(bool);
    virtual void startPlaying();
    virtual void hit(Shot* shot);
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    virtual void notifyChat(const Entity* entity, const QString& text);
    virtual void notifyNick(Sea::Player player, const QString& text);
    virtual void notifyGameOptions(bool ask) { };
    // delegate interface
    virtual void action(Sea::Player player, const Coord& c);
    virtual void changeDirection(Sea::Player player);
    virtual bool canAddShip(Sea::Player player, const Coord& c);
    virtual Ship * nextShip();
    
    virtual void setNick(const QString& nick);
protected:
    // parent interface
    virtual void registerHit(Sea::Player player, const Coord& c);
    virtual void registerMiss(Sea::Player player, const Coord& c);
public slots:
    virtual void notifyAbort();
    virtual void notifyRestartPlacing(Sea::Player player);
};

#endif // PLAYERENTITY_H
