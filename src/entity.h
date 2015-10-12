/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>

#include "sea.h"
#include "seaview.h"
#include "stats.h"
#include "ships.h"

class Shot;
class QIcon;

class Entity : public QObject
{
Q_OBJECT
public:
    enum CompatibilityLevel
    {
        COMPAT_KBS3,
        COMPAT_KBS4
    };
protected:
    Sea::Player m_player;
    SeaView* m_seaview;
    QString m_nick;
    CompatibilityLevel m_level;
    Stats m_stats;
    // not owned by the entity
    const BattleShipsConfiguration* m_battleShipsConfiguration;
    bool m_restarted;

public:
    Entity(Sea::Player player, SeaView* seaview, const BattleShipsConfiguration* battleShipsConfiguration);
    virtual ~Entity();
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info) = 0;
    virtual void notifyChat(const Entity* entity, const QString& text) = 0;
    virtual void notifyNick(Sea::Player player, const QString& nick) = 0;
    virtual void hit(Shot* shot) = 0;
    virtual void notifyGameOptions() = 0;
    virtual void startPlacing() = 0;
    virtual void start() = 0;
    virtual void startPlaying() { }
    virtual void notifyReady(Sea::Player) { }
    virtual void notifyShips(Sea::Player) { }
    virtual void notifyGameOver(Sea::Player) { }

    Stats* stats();
    
    virtual Sea::Player player() const { return m_player; }
    
    QString nick() const { return m_nick; }
    CompatibilityLevel compatibilityLevel() const { return m_level; }
    virtual QIcon icon() const = 0;
    virtual void setNick(const QString& nick);
signals:
    void shoot(int player, const Coord& c);
    void shipsPlaced();
    void ready(int player);
    void chat(const QString& text);
    void nickChanged(int player, const QString& nickname);
    void compatibility(int level);
    void abortGame();
    void restartPlacingShips(Sea::Player player);
    void gameOptionsInterchanged();
public slots:
    virtual void setCompatibilityLevel(int level);
    virtual void notifyAbort() = 0;
    virtual void notifyRestartPlacing(Sea::Player player) = 0;
};

#endif // ENTITY_H

