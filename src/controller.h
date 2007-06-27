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
class Stats;
class Shot;
class AudioPlayer;

class Controller : public QObject
{
Q_OBJECT
    QList<Entity*> m_entities;
    UIEntity* m_ui;
    Sea* m_sea;
    Shot* m_shot;
    int m_ready;
    AudioPlayer* m_player;
    
    void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    void setupEntity(Entity*);
    Entity* findEntity(Sea::Player) const;
    void finalizeShot(Sea::Player player, const Coord& c, const HitInfo& info);
    bool allPlayers() const;

    
    friend class Shot;
public:
    explicit Controller(QObject* parent, AudioPlayer* player = 0);

    PlayerEntity* createPlayer(Sea::Player player, SeaView* view, 
                               ChatWidget* chat, const QString& nick);
    AIEntity* createAI(Sea::Player player);
    NetworkEntity* createRemotePlayer(Sea::Player player, QIODevice* device, bool client);
    
    bool start(SeaView* view);
public slots:
    void shoot(int player, const Coord& c);
    void ready(int player);
    void receivedChat(const QString& text);
    void nick(int player, const QString& nick);
signals:
    void gameOver(Sea::Player);
};

#endif // CONTROLLER_H

