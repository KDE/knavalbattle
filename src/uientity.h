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
    ~UIEntity() override;
    
    void notify(Sea::Player player, const Coord& c, const HitInfo& info) override;
    void notifyGameOver(Sea::Player winner) override;
    void notifyChat(const Entity*, const QString&) override { }
    void notifyNick(Sea::Player, const QString&) override { }
    void notifyGameOptions() override { emit gameOptionsInterchanged(); }
    void start(bool) override;
    void startPlacing() override { };
    void hit(Shot*) override;
    
    virtual void registerHit(Sea::Player, const Coord&) { }
    virtual void registerMiss(Sea::Player, const Coord&) { }
    
    QIcon icon() const override;
public slots:
    void notifyAbort() override { }
    void notifyRestartPlacing(Sea::Player player) override { };
};

#endif // UIENTITY_H
