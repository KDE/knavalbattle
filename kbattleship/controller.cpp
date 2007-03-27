/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <kdebug.h>
#include "controller.h"
#include "seaview.h"

// for rand
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

Controller::Controller(QObject* parent)
: QObject(parent)
{
}

OnePlayerController::OnePlayerController(QObject* parent, SeaView* view, Sea::Player player)
: Controller(parent)
, m_sea(0)
, m_view(view)
, m_player(player)
{
    reset();
}

Ship* OnePlayerController::canAddShip(Sea::Player player, const Coord& c)
{
    Ship* next = nextShip();
    if (next == 0 || player != m_player) {
        return 0;
    }
    if (m_sea->canAddShip(player, c, next->size(), next->direction())) {
        return next;
    }
    else {
        return 0;
    }
}

void OnePlayerController::hit(Sea::Player player, const Coord& c)
{
    Sea::Player opponent = Sea::opponent(player);
    if (m_sea->canHit(player, c)) {
        HitInfo info = m_sea->hit(c);
        switch (info.type) {
        case HitInfo::HIT:
            m_view->hit(opponent, c);
            break;
        case HitInfo::MISS:
            m_view->miss(opponent, c);
            break;
        case HitInfo::INVALID:
            break;
        }
            
        if (info.shipDestroyed && player == m_player) {
            // show destroyed ship
            Coord c = m_sea->find(opponent, info.shipDestroyed);
            if (c.valid()) {
                m_view->add(opponent, c, info.shipDestroyed);
            }
        }
    }
}

void OnePlayerController::action(Sea::Player player, const Coord& c)
{
    if (nextShip()) {
        // placing ships
        Ship* ship = canAddShip(player, c);
        if (ship) {
            
            // remove ship from the list
            m_ships.removeFirst();
            
            // add ship to the sea
            m_sea->add(player, c, ship);
            m_view->add(player, c, ship);
            m_view->cancelPreview();
            
            if (!nextShip()) {
                m_sea->startPlaying();
            }
        }
    }
    else {
        // here player must be the opponent
        if (Sea::opponent(m_player) == player) {
            hit(m_player, c);
            
            // computer answers        
            while (m_sea->turn() == player &&
                    m_sea->status() == Sea::PLAYING) {
                Coord c(rand() % 10, rand() % 10);
                hit(player, c);
            }
            
            if (m_sea->status() != Sea::PLAYING) {
                kDebug() << "game over" << endl;
                m_view->gameOver(m_sea->status());
            }
        }
    
        
    }
}

void OnePlayerController::changeDirection(Sea::Player p)
{
    if (p == m_player) {
        Ship* next = nextShip();
        if (next) {
            next->changeDirection();
            m_view->cancelPreview();
        }
    }
}

void OnePlayerController::reset()
{
    m_view->clear();
    
    // create model
    delete m_sea;
    m_sea = new Sea(this, Coord(10, 10));
    
    // set up player ships
    for (int i = 1; i <= 4; i++) {
        m_ships.append(new Ship(i, Ship::LEFT_TO_RIGHT));
    }
        
    // set up computer ships
    srand(time(0));
    for (int i = 1; i <= 4; i++) {
        Ship* ship = 0;
        Sea::Player p = Sea::opponent(m_player);
        while (ship == 0) {
            Coord c(rand() % 10, rand() % 10);
            Ship::Direction dir = rand() % 2 == 0 ? Ship::LEFT_TO_RIGHT : Ship::TOP_DOWN;
            if (m_sea->canAddShip(p, c, i, dir)) {
                ship = new Ship(i, dir);
                m_sea->add(p, c, ship);
            }
        }   
    }
}

Ship* OnePlayerController::nextShip()
{
    if (m_ships.isEmpty()) {
        return 0;
    }
    else {
        return m_ships.first();
    }
}

#include "controller.moc"
