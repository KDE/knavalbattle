/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QTimer>
#include "sea.h"

class SeaView;
class AI;

class Controller : public QObject
{
Q_OBJECT
public:
    Controller(QObject* parent);
    virtual ~Controller() { }
    virtual Ship* canAddShip(Sea::Player, const Coord& c) = 0;
    virtual void action(Sea::Player, const Coord& c) = 0;
    virtual void changeDirection(Sea::Player) = 0;
};

class OnePlayerController : public Controller
{
Q_OBJECT
    QList<Ship*> m_ships;
    Sea* m_sea;
    SeaView* m_view;
    Sea::Player m_player;
    AI* m_ai;
    
    void hit(Sea::Player player, const Coord& c);
    bool checkGameOver();
public:
    OnePlayerController(QObject* parent, SeaView* view, Sea::Player player);
    
    virtual Ship* canAddShip(Sea::Player, const Coord& c);
    virtual void action(Sea::Player, const Coord& c);
    virtual void changeDirection(Sea::Player);
    
    Ship* nextShip();
    void reset();
};

class TwoMachinesController : public Controller
{
Q_OBJECT
    Sea* m_sea;
    SeaView* m_view;
    AI* m_ai[2];
    QTimer m_timer;
    
    void play(Sea::Player player);
    void hit(Sea::Player player, const Coord& c);
private slots:
    void tick();
public:
    TwoMachinesController(QObject* parent, SeaView* view);
    
    virtual Ship* canAddShip(Sea::Player, const Coord&) { return 0; }
    virtual void action(Sea::Player, const Coord&) { }
    virtual void changeDirection(Sea::Player) { }
};

#endif // CONTROLLER_H

