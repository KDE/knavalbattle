/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sea.h"

class Entity;
class AIEntity;
class NetworkEntity;
class UIEntity;
class PlayerEntity;
class SeaView;
class ChatWidget;
class Shot;
class AudioPlayer;
class Protocol;
class BattleShipsConfiguration;

class Controller : public QObject
{
Q_OBJECT
    QList<Entity*> m_entities;
    UIEntity* m_ui;
    Sea* m_sea;
    Shot* m_shot;
    int m_ready;
    AudioPlayer* m_player;
    bool m_has_ai;
    BattleShipsConfiguration mBattleShipsConfiguration;

    void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    void setupEntity(Entity*);
    void finalizeShot(Sea::Player player, const Coord& c, const HitInfo& info);
    void finalizeGame(Sea::Player winner);
    bool allPlayers() const;


    friend class Shot;
public:
    explicit Controller(QObject* parent, AudioPlayer* audioPlayer = 0, const BattleShipsConfiguration& battleConfiguration = BattleShipsConfiguration::defaultSingleShipsConfiguration(true));

    PlayerEntity* createPlayer(Sea::Player player, SeaView* view,
                               ChatWidget* chat, const QString& nick);
    AIEntity* createAI(Sea::Player player, SeaView* view);
    NetworkEntity* createRemotePlayer(Sea::Player player, SeaView* view, Protocol* protocol, bool client);

    bool start(SeaView* view);
    Entity* findEntity(Sea::Player) const;
    Sea::Player turn() const;
    bool hasAI() const;
    inline Sea* getSea() const { return m_sea; }
    void setBattleShipsConfiguration(const BattleShipsConfiguration& battleConfiguration);
    inline BattleShipsConfiguration& getBattleShipsConfiguration() { return mBattleShipsConfiguration; }
public slots:
    void shoot(int player, const Coord& c);
    void ready(int player);
    void shipsPlaced();
    void receivedChat(const QString& text);
    void nick(int player, const QString& nick);
    void notifyRestartPlacingShips(Sea::Player player);
    void placing();
    void restart();
signals:
    void gameOver(Sea::Player);
    void restartRequested();
    void startPlacingShips(int player);
    void restartPlacingShips(Sea::Player player); // in case it is impossible to finish with the current board
    void compatibility(int);
    void nickChanged(int, const QString&);
    void turnChanged(int);
    void playerReady(int); // -1 means all players are ready
};

#endif // CONTROLLER_H

