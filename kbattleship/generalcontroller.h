/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef GENERALCONTROLLER_H
#define GENERALCONTROLLER_H

#include "sea.h"

class Entity;
class UIEntity;
class SeaView;
class Stats;

class GeneralController : public QObject
{
Q_OBJECT
    QList<Entity*> m_entities;
    UIEntity* m_ui;
    Sea* m_sea;
    int m_ready;
    
    void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    void setupEntity(Entity*);
    Entity* findEntity(Sea::Player) const;
public:
    GeneralController(QObject* parent);

    void createPlayer(Sea::Player player, SeaView* view);
    void createAI(Sea::Player player);
    const Stats* stats() const;
    
    void start(SeaView* view);
public slots:
    void shoot(Sea::Player player, const Coord& c);
    void ready(Sea::Player player);
signals:
    void gameOver(Sea::Player);
};

#endif // GENERALCONTROLLER_H

