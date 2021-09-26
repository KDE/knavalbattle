/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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


    friend class DelayedShot;
public:
    AIEntity(Sea::Player player, Sea* sea, SeaView *seaview);
    ~AIEntity() override;

    void notify(Sea::Player player, const Coord& c, const HitInfo& info) override;
    void notifyChat(const Entity*, const QString&) override { }
    void notifyNick(Sea::Player, const QString&) override { }
    void start() override;
    void startPlacing() override;
    void startPlaying() override;
    void hit(Shot* shot) override;
    void notifyGameOptions() override;

    QIcon icon() const override;
public Q_SLOTS:
    void notifyAbort() override { }
    void notifyRestartPlacing(Sea::Player) override { }

private Q_SLOTS:
    void getShoot();
};


#endif // AIENTITY_H

