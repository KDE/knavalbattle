/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "message.h"

HeaderMessage::HeaderMessage(const QString& protocolVersion,
                             const QString& clientName,
                             const QString& clientVersion,
                             const QString& clientDescription)
: m_protocol_version(protocolVersion)
, m_client_name(clientName)
, m_client_version(clientVersion)
, m_client_description(clientDescription)
{
}

HeaderMessage::HeaderMessage()
: m_protocol_version("0.1.0")
, m_client_name("KBattleship")
, m_client_version("4.0")
, m_client_description("The Naval Battle game")
{
}

void HeaderMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}

RejectMessage::RejectMessage(bool versionMismatch, const QString& reason)
: m_version_mismatch(versionMismatch)
, m_reason(reason)
{
}

void RejectMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}

NickMessage::NickMessage(const QString& nickname)
: m_nickname(nickname)
{
}

void NickMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}


void BeginMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}

MoveMessage::MoveMessage(const Coord& move)
: m_move(move)
{
}

void MoveMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}

NotificationMessage::NotificationMessage(const Coord& move,
                                         bool hit, bool death,
                                         const Coord& start, const Coord& stop)
: m_move(move)
, m_hit(hit)
, m_death(death)
, m_start(start)
, m_stop(stop)
{
}

void NotificationMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}

ChatMessage::ChatMessage(const QString& nickname, const QString& chat)
: m_nickname(nickname)
, m_chat(chat)
{
}

void ChatMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}


void RestartMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}

GameOverMessage::GameOverMessage()
{
}

void GameOverMessage::addShip(const Coord& pos, int size, Ship::Direction direction)
{
    m_ships.append(ShipInfo(pos, size, direction));
}

void GameOverMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}

GameOptionsMessage::GameOptionsMessage(const QString& enableAdjacentShips, const QString& oneOrSeveralShips,
    const BattleShipsConfiguration* configuration)
: m_enabledAdjacentShipsString(enableAdjacentShips)
, m_oneOrSeveralShipsString(oneOrSeveralShips)
, m_battleShipsConfiguration(configuration)
{
}

//  TODO: Several valgrind sessions. Those problems does not exist in python
GameOptionsMessage::GameOptionsMessage(const bool enableAdjacentShips, const bool oneOrSeveralShips,
    BattleShipsConfiguration configuration)
: m_enabledAdjacentShipsString(enableAdjacentShips)
, m_oneOrSeveralShipsString(oneOrSeveralShips)
, m_battleShipsConfiguration(new BattleShipsConfiguration(configuration))
{
}

void GameOptionsMessage::accept(MessageVisitor& visitor) const
{
    visitor.visit(*this);
}
