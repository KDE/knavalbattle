/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "networkentity.h"
#include "shot.h"
#include "protocol.h"
#include <QIODevice>

NetworkEntity::NetworkEntity(Sea::Player player, Sea* sea, QIODevice* device)
: Entity(player)
, m_sea(sea)
{
    m_protocol = new Protocol(device);
}

NetworkEntity::~NetworkEntity()
{
    foreach (Ship* ship, m_destroyed_ships) {
        delete ship;
    }
}

void NetworkEntity::start()
{
    m_protocol->send(HeaderMessage());
    m_protocol->send(NickMessage("dude"));
}

void NetworkEntity::notifyReady(Sea::Player player)
{
    if (player != m_player) {
        m_protocol->send(BeginMessage());
    }
}

void NetworkEntity::startPlaying()
{

}

void NetworkEntity::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    if (info.type == HitInfo::INVALID) {
        return;
    }
    
    if (player == m_player) {
        bool hit = info.type == HitInfo::HIT;
        bool death = info.shipDestroyed != 0;
        Coord begin = Coord::invalid();
        Coord end = Coord::invalid();
        if (death) {
            begin = info.shipPos;
            end = begin + info.shipDestroyed->increment() * info.shipDestroyed->size();
        }
        m_protocol->send(NotificationMessage(c, hit, death, begin, end));
    }
    else {
        // the remote player already knows about the hit
        // no need to notify it
    }
}

void NetworkEntity::hit(Shot* shot)
{
    if (shot->player() != m_player && m_sea->turn() == shot->player()) {
        m_pending_shot = shot;
        m_protocol->send(MoveMessage(shot->pos()));
    }
}

void NetworkEntity::received(MessagePtr msg)
{
    msg->accept(*this);
}

void NetworkEntity::visit(const HeaderMessage&)
{
    
}

void NetworkEntity::visit(const RejectMessage&)
{

}

void NetworkEntity::visit(const NickMessage& msg)
{
    m_nick = msg.nickname();
}

void NetworkEntity::visit(const BeginMessage&)
{
    emit ready(m_player);
}

void NetworkEntity::visit(const MoveMessage&)
{
}

void NetworkEntity::visit(const NotificationMessage& msg)
{
    if (m_pending_shot) {
        if (m_pending_shot->pos() != msg.move()) {
            m_pending_shot->execute(HitInfo::INVALID);
        }
        else {
            HitInfo info = msg.hit() ? HitInfo::HIT : HitInfo::MISS;
            if (msg.death()) {
                // gather ship data
                Coord delta = msg.stop() - msg.start();
                int size = abs(delta.x) + abs(delta.y);
                Ship::Direction direction = delta.x == 0 ? Ship::TOP_DOWN : Ship::LEFT_TO_RIGHT;
                Coord shipPos = (delta.x < 0 || delta.y < 0) ? msg.stop() : msg.start();
                Ship* ship = new Ship(size, direction);
                m_destroyed_ships.append(ship);
                
                info.shipDestroyed = ship;
                info.shipPos = shipPos;
            }
            
            m_sea->switchTurn();
            m_pending_shot->execute(info);
        }
        
        m_pending_shot = 0;
    }
}

void NetworkEntity::visit(const GameOverMessage&)
{

}

void NetworkEntity::visit(const RestartMessage&)
{

}

void NetworkEntity::visit(const ChatMessage&)
{

}

#include "networkentity.moc"


