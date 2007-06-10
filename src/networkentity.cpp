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

NetworkEntity::NetworkEntity(Sea::Player player, Sea* sea, QIODevice* device, bool client)
: Entity(player)
, m_sea(sea)
, m_client(client)
{
    m_protocol = new Protocol(device);
}

NetworkEntity::~NetworkEntity()
{

}

void NetworkEntity::start()
{
    connect(m_protocol, SIGNAL(received(MessagePtr)), this, SLOT(received(MessagePtr)));
//     if (m_client) {
        m_protocol->send(MessagePtr(new HeaderMessage()));
//     }
}

void NetworkEntity::notifyReady(Sea::Player player)
{
    if (player != m_player) {
        m_protocol->send(MessagePtr(new BeginMessage()));
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
            end = begin + info.shipDestroyed->increment() * (info.shipDestroyed->size() - 1);
        }
        m_protocol->send(MessagePtr(new NotificationMessage(c, hit, death, begin, end)));
    }
    else {
        // the remote player already knows about the hit
        // no need to notify it
    }
}

void NetworkEntity::notifyNick(Sea::Player player, const QString& nick)
{
    if (player != m_player) {
        m_protocol->send(MessagePtr(new NickMessage(nick)));
    }
}

void NetworkEntity::notifyChat(const QString& nickname, const QString& text)
{
    if (nickname != nick()) {
        m_protocol->send(MessagePtr(new ChatMessage(nickname, text)));
    }
}

void NetworkEntity::hit(Shot* shot)
{
    if (shot->player() != m_player 
        && m_sea->turn() == shot->player()
        && m_sea->valid(m_player, shot->pos())) {
        m_pending_shot = shot;
        m_protocol->send(MessagePtr(new MoveMessage(shot->pos())));
    }
    else {
        shot->execute(HitInfo::INVALID);
    }
}

void NetworkEntity::received(MessagePtr msg)
{
    msg->accept(*this);
}

void NetworkEntity::visit(const HeaderMessage& msg)
{
    if (msg.clientName() == "KBattleship" && msg.clientVersion().toFloat() >= 4.0) {
        // m_level = COMPAT_KBS4;
    }
    else {
        m_level = COMPAT_KBS3;
        kDebug() << "emitting compatibility" << endl;
        emit compatibility(m_level);
    }
}

void NetworkEntity::visit(const RejectMessage&)
{

}

void NetworkEntity::visit(const NickMessage& msg)
{
    setNick(msg.nickname());
    emit nick(m_player, m_nick);
}

void NetworkEntity::visit(const BeginMessage&)
{
    m_sea->add(m_player, 4);
    emit ready(m_player);
}

void NetworkEntity::visit(const MoveMessage& msg)
{
    kDebug() << m_player << ": shooting on " << msg.move() << endl;
    emit shoot(m_player, msg.move());
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
                int size = abs(delta.x) + abs(delta.y) + 1;
                Ship::Direction direction = delta.x == 0 ? Ship::TOP_DOWN : Ship::LEFT_TO_RIGHT;
                Coord shipPos = (delta.x < 0 || delta.y < 0) ? msg.stop() : msg.start();
                Ship* ship = new Ship(size, direction);
                
                info.shipDestroyed = ship;
                info.shipPos = shipPos;
            }
            
            m_sea->forceHit(msg.move(), info);
            if (m_level == COMPAT_KBS3 && info.shipDestroyed) {
                m_sea->addBorder(m_player, info.shipPos);
            }
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

void NetworkEntity::visit(const ChatMessage& msg)
{
    emit chat(msg.nickname(), msg.chat());
}

#include "networkentity.moc"


