/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AIENTITY_H
#define AIENTITY_H

#include "entity.h"

class AI;

class AIEntity : public Entity
{
Q_OBJECT
    AI* m_ai;
    Sea* m_sea;

    void getShoot();

    friend class DelayedShot;
public:
    AIEntity(Sea::Player player, Sea* sea, SeaView *seaview);
    ~AIEntity() override;

    void notify(Sea::Player player, const Coord& c, const HitInfo& info) override;
    void notifyChat(const Entity*, const QString&) override { }
    void notifyNick(Sea::Player, const QString&) override { }
    void start(bool) override;
    void startPlacing() override;
    void startPlaying() override;
    void hit(Shot* shot) override;
    void notifyGameOptions() override;

    QIcon icon() const override;
public slots:
    void notifyAbort() override { }
    void notifyRestartPlacing(Sea::Player player) override { }
};


#endif // AIENTITY_H

