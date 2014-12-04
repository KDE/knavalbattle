/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "networkentity.h"

#include "battlefield.h"
#include "shot.h"
#include "protocol.h"
#include "settings.h"
#include "seaview.h"

#include <QIcon>

#include <KLocalizedString>

NetworkEntity::NetworkEntity(Sea::Player player, Sea* sea, SeaView* seaview, Protocol* protocol, bool client)
: Entity(player, seaview, sea->battleShipsConfiguration())
, m_sea(sea)
, m_protocol(protocol)
, m_pending_shot(0)
, m_client(client)
, m_winner(false)
{
}

NetworkEntity::~NetworkEntity()
{
}

void NetworkEntity::start(bool ask)
{
}

void NetworkEntity::notifyReady(Sea::Player player)
{
    if (player != m_player) {
        m_protocol->send(MessagePtr(new BeginMessage()));
    }
}

void NetworkEntity::notifyShips(Sea::Player player)
{
    m_winner= player == m_player ;

    GameOverMessage* msg=new GameOverMessage();
    if (!m_winner) {
        foreach (Ship* ship, m_sea->myShips()) {
            if (ship->alive()) {
                msg->addShip(ship->position(), ship->size(), ship->direction());
            }
        }
    }
    m_protocol->send(MessagePtr(msg));
}

void NetworkEntity::notifyGameOver(Sea::Player player)
{
}

void NetworkEntity::notifyRestart(Sea::Player)
{
    if (!m_restarted) {
        m_protocol->send(MessagePtr(new RestartMessage()));
        m_restarted = true;
    }
}


void NetworkEntity::notifyGameOptions(bool ask)
{
    connect(m_protocol, &Protocol::received, this, &NetworkEntity::received);
    connect(m_protocol, &Protocol::disconnected, this, &NetworkEntity::abortGame);
    if (ask || m_restarted) {
        m_protocol->send(MessagePtr(new RestartMessage()));
        m_restarted = true;
    }
    else {
        m_protocol->send(MessagePtr(new HeaderMessage()));
        
        m_protocol->send(MessagePtr(new GameOptionsMessage(QString(Settings::adjacentShips() ? "true" : "false"), QString(Settings::severalShips() ? "true" : "false"), m_battleShipsConfiguration )));
    }
}

void NetworkEntity::startPlacing(bool ask)
{
    m_restarted = false;
    m_sea->clear(m_player);

    // Number of ships to sink again in the new game
    m_sea->add(m_player, m_battleShipsConfiguration->totalNumberOfShipsToPlay());

    emit ready(m_player);
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

void NetworkEntity::notifyChat(const Entity* entity, const QString& text)
{
    if (entity != this) {
        m_protocol->send(MessagePtr(new ChatMessage(entity->nick(), text)));
    }
}

void NetworkEntity::notifyAbort()
{
    // TODO: close connection
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
    if (msg)
      msg->accept(*this);
}

void NetworkEntity::visit(const HeaderMessage& msg)
{
    if (msg.clientName() == "KBattleship" && msg.clientVersion().toFloat() >= 4.0) {
        // m_level = COMPAT_KBS4;
    }
    else {
        if (m_level != COMPAT_KBS3) {
            m_level = COMPAT_KBS3;
            emit compatibility(m_level);
        }
    }
}

void NetworkEntity::visit(const RejectMessage&)
{

}

void NetworkEntity::visit(const NickMessage& msg)
{
    setNick(msg.nickname());
    emit nick(m_player, m_nick);
    // This is a dirty hack caused by the introduction of GameOptionsMessage.
    // If that had extended BeginMessage, the following instructions will
    // be in the right place.
    // It is done here because the nickMessage is sent after GameOptionsMessage
    // (if sent) and before start placing ships.
    if ( !m_battleShipsConfiguration->isFromXML() && !m_restarted)
    {
        m_sea->setBattleShipsConfiguration(BattleShipsConfiguration::defaultSingleShipsConfiguration(true));
        m_battleShipsConfiguration = m_sea->battleShipsConfiguration();
        // TODO: Message explaining why the network game uses this configuration
    }
    // form the chat message telling the number of ships of each type to place and shink
    QString message=i18n("You have ");
    bool comma=false;
    for (unsigned int size = 1; size <= m_battleShipsConfiguration->longestShip(); size++)
    {
        if (comma)
        {
            message.append(i18n(", "));
        }
        comma=true;
        if ( m_battleShipsConfiguration->numberOfShipsOfSize(size) == 1 )
        {
            message.append(QLatin1String("1 ")).append(m_battleShipsConfiguration->nameOfShipsOfSize(size));
        }
        else
        {
            message.append(QString::number(m_battleShipsConfiguration->numberOfShipsOfSize(size)))
                   .append(" ")
                   .append(i18n(m_battleShipsConfiguration->pluralNameOfShipsOfSize(size).toLatin1()));
        }
    }
    emit chat(message);
    emit gameOptionsInterchanged();
    // Number of ships to sink
    m_sea->add(m_player, m_battleShipsConfiguration->totalNumberOfShipsToPlay());
}

void NetworkEntity::visit(const BeginMessage&)
{
    emit ready(m_player);
}

void NetworkEntity::visit(const MoveMessage& msg)
{
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
                Ship* ship = new Ship(size, direction, shipPos);
                
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

void NetworkEntity::visit(const GameOverMessage& msg)
{
    {
        // receive the ships and add them to the board
        foreach (GameOverMessage::ShipInfo ship, msg.ships()) {
                m_seaview->add(m_sea->turn(), new Ship(ship.size, ship.direction, ship.pos));
        }
    }
}

void NetworkEntity::visit(const RestartMessage&)
{
    // Keep the current configuration in a restarted game.
    if (!m_restarted)
    {
        emit restartRequested();
    }
}

void NetworkEntity::visit(const ChatMessage& msg)
{
    emit chat(msg.chat());
}

void NetworkEntity::visit(const GameOptionsMessage& msg)
{
    bool enabledAdjacentShips = (msg.enabledAdjacentShips() == QString("true"));
    if (m_client) {
        m_sea->allowAdjacentShips( enabledAdjacentShips );
    }

    if (m_sea->isAdjacentShipsAllowed()) {
        emit chat(i18n("You can place ships adjacent to each other"));
    }
    else {
        emit chat(i18n("You must leave a space between ships"));
    }

    if (m_client)
    {
        m_battleShipsConfiguration = msg.shipsConfiguration();
        m_sea->setBattleShipsConfiguration(*m_battleShipsConfiguration);
    }
    else
    {
        if (msg.shipsConfiguration()->isFromXML())
        {
            // if the client understood GameOptions, then the global preferences in the server are used.
            const_cast<BattleShipsConfiguration*>(m_battleShipsConfiguration)->setFromXML(true);
         }
    }
}


QIcon NetworkEntity::icon() const
{
    return QIcon( QLatin1String( "network-workgroup" ));
}


