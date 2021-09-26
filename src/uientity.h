/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    void notifyGameOptions() override { Q_EMIT gameOptionsInterchanged(); }
    void start() override;
    void startPlacing() override { };
    void hit(Shot*) override;
    
    virtual void registerHit(Sea::Player, const Coord&) { }
    virtual void registerMiss(Sea::Player, const Coord&) { }
    
    QIcon icon() const override;
public Q_SLOTS:
    void notifyAbort() override { }
    void notifyRestartPlacing(Sea::Player) override { };
};

#endif // UIENTITY_H
