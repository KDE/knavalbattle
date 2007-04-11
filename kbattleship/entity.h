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

class Stats;

class Entity : public QObject
{
Q_OBJECT
protected:
    Sea::Player m_player;
public:
    Entity(Sea::Player player);
    virtual ~Entity();
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info) = 0;
    virtual void start() = 0;
    virtual void startPlaying() { }
    virtual const Stats* stats() const { return 0; }
signals:
    void shoot(Sea::Player player, const Coord& c);
    void ready(Sea::Player player);
};

#endif // ENTITY_H

