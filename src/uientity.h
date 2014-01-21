/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef UIENTITY_H
#define UIENTITY_H

#include "entity.h"

class SeaView;

class UIEntity : public Entity
{
Q_OBJECT
protected:
    Sea* m_sea;

    void drawShoot(Sea::Player player, const Coord& c, const HitInfo& info);
    void drawHiddenShips(Sea::Player winner);
public:
    UIEntity(Sea::Player player, Sea*, SeaView* view);
    virtual ~UIEntity();
    
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    virtual void notifyGameOver(Sea::Player winner);
    virtual void notifyChat(const Entity*, const QString&) { }
    virtual void notifyNick(Sea::Player, const QString&) { }
    virtual void notifyGameOptions(bool ask) { emit gameOptionsInterchanged(); }
    virtual void start(bool);
    virtual void startPlacing(bool) { };
    virtual void hit(Shot*);
    
    virtual void registerHit(Sea::Player, const Coord&) { }
    virtual void registerMiss(Sea::Player, const Coord&) { }
    
    virtual KIcon icon() const;
public slots:
    virtual void notifyAbort() { }
    virtual void notifyRestartPlacing(Sea::Player player) { };
};

#endif // UIENTITY_H
