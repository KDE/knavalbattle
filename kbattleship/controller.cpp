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
#include "ai/ai.h"
#include "ai/smartai.h"
#include "ai/dummyai.h"

Controller::Controller(QObject* parent)
: QObject(parent)
{
}

OnePlayerController::OnePlayerController(QObject* parent, SeaView* view, Sea::Player player)
: Controller(parent)
, m_sea(0)
, m_view(view)
, m_player(player)
, m_ai(0)
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
        m_ai->notify(player, c, info);
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
        if (Sea::opponent(m_player) == player && m_sea->canHit(m_player, c)) {
            hit(m_player, c);
            
            if (checkGameOver())
                return;
            
            Coord mv = m_ai->getMove();
            if (m_sea->canHit(Sea::opponent(m_player), mv)) {
                hit(Sea::opponent(m_player), mv);
            }
            else {
                kDebug() << "ERROR: Computer played an invalid move " << mv << endl;
                m_sea->abort(m_player);
            }
            
            if (checkGameOver())
                return;
        }
    
        
    }
}

bool OnePlayerController::checkGameOver()
{
    if (m_sea->status() != Sea::PLAYING) {
        kDebug() << "game over" << endl;
        m_view->gameOver(m_sea->status());
        return true;
    }
    return false;
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
        
    // create ai
    delete m_ai;
    m_ai = new SmartAI(Sea::opponent(m_player), m_sea);
    
    // set up computer ships
    m_ai->setShips();
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

TwoMachinesController::TwoMachinesController(QObject* parent, SeaView* view)
: Controller(parent)
, m_view(view)
{
    m_view->clear();
    m_sea = new Sea(this, Coord(10, 10)); 
    
    m_ai[0] = new DummyAI(Sea::PLAYER_A, m_sea);
    m_ai[1] = new SmartAI(Sea::PLAYER_B, m_sea);
    
    m_ai[0]->setShips();
    m_ai[1]->setShips();
    
    m_sea->startPlaying();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_timer.start(0);
}

void TwoMachinesController::play(Sea::Player player)
{
    Coord c = m_ai[player]->getMove();
    hit(player, c);
}

void TwoMachinesController::tick()
{
    if (m_sea->status() == Sea::PLAYING) {
        play(m_sea->turn());
    }
    else {
        m_timer.stop();
        if (m_sea->status() == Sea::A_WINS) {
            kDebug() << "dummy wins" << endl;
        }
        else {
            kDebug() << "smart wins" << endl;
        }
    }
}

void TwoMachinesController::hit(Sea::Player player, const Coord& c)
{
    Sea::Player opponent = Sea::opponent(player);
    if (m_sea->canHit(player, c)) {
        HitInfo info = m_sea->hit(c);
        m_ai[player]->notify(player, c, info);
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
            
        if (info.shipDestroyed) {
            // show destroyed ship
            Coord c = m_sea->find(opponent, info.shipDestroyed);
            if (c.valid()) {
                m_view->add(opponent, c, info.shipDestroyed);
            }
        }
    }
}


#include "controller.moc"
