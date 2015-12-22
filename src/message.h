/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QExplicitlySharedDataPointer>

#include "ship.h"
#include "ships.h"

class MessageVisitor;

class Message : public QSharedData
{
public:
    virtual ~Message() { }
    virtual void accept(MessageVisitor& visitor) const = 0;
};

typedef QExplicitlySharedDataPointer<Message> MessagePtr;

class HeaderMessage : public Message
{
    QString m_protocol_version;
    QString m_client_name;
    QString m_client_version;
    QString m_client_description;
public:
    static const int MSGTYPE = 0;
    HeaderMessage(const QString& protocolVersion,
                 const QString& clientName,
                 const QString& clientVersion,
                 const QString& clientDescription);
    HeaderMessage();
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("Header"); }
    
    const QString& protocolVersion() const { return m_protocol_version; }
    const QString& clientName() const { return m_client_name; }
    const QString& clientVersion() const { return m_client_version; }
    const QString& clientDescription() const { return m_client_description; }
};

class RejectMessage : public Message
{
    bool m_version_mismatch;
    QString m_reason;
public:
    static const int MSGTYPE = 1;
    RejectMessage(bool versionMismatch, const QString& reason);
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("Reject"); }
};

class NickMessage : public Message
{
    QString m_nickname;
public:
    static const int MSGTYPE = 2;
    explicit NickMessage(const QString& nickname);
    void accept(MessageVisitor& visitor) const override;
    
    const QString& nickname() const { return m_nickname; }
    static QString messageType() { return QLatin1String("Nick"); }
};

class BeginMessage : public Message
{
public:
    static const int MSGTYPE = 3;
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("Begin"); }
};

class MoveMessage : public Message
{
    Coord m_move;
public:
    static const int MSGTYPE = 4;
    explicit MoveMessage(const Coord& move);
    void accept(MessageVisitor& visitor) const override;
    
    const Coord& move() const { return m_move; }
    static QString messageType() { return QLatin1String("Move"); }
};

class NotificationMessage : public Message
{
    Coord m_move;
    bool m_hit;
    bool m_death;
    Coord m_start;
    Coord m_stop;
public:
    static const int MSGTYPE = 5;
    NotificationMessage(const Coord& m_move, bool hit, bool death,
        const Coord& start = Coord::invalid(), const Coord& stop = Coord::invalid());
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("Notification"); }
    
    const Coord& move() const { return m_move; }
    bool hit() const { return m_hit; }
    bool death() const { return m_death; }
    const Coord& start() const { return m_start; }
    const Coord& stop() const { return m_stop; }
};

class GameOverMessage : public Message
{
public:
    struct ShipInfo
    {
        Coord pos;
        int size;
        Ship::Direction direction;

        ShipInfo(const Coord& pos, int size, Ship::Direction direction)
        : pos(pos)
        , size(size)
        , direction(direction)
        {
        }
    };
private:
    QList<ShipInfo> m_ships;
public:
    static const int MSGTYPE = 6;
    GameOverMessage();

    void addShip(const Coord& pos, int size, Ship::Direction direction);
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("GameOver"); }

    const QList<ShipInfo>& ships() const { return m_ships; }
};

class RestartMessage : public Message
{
public:
    static const int MSGTYPE = 7;
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("Restart"); }
};

class ChatMessage : public Message
{
    QString m_nickname;
    QString m_chat;
public:
    static const int MSGTYPE = 8;
    explicit ChatMessage(const QString& nick, const QString& chat);
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("Chat"); }

    const QString& chat() const { return m_chat; }
    const QString& nickname() const { return m_nickname; }
};


class GameOptionsMessage : public Message
{
private:
    QString m_enabledAdjacentShipsString;
    QString m_oneOrSeveralShipsString;
    const BattleShipsConfiguration* m_battleShipsConfiguration;
public:
    static const int MSGTYPE = 9;
    GameOptionsMessage(const QString& enableAdjacentShips, const QString& oneOrSeveralShips,
        const BattleShipsConfiguration* configuration);
    GameOptionsMessage(const bool enableAdjacentShips, const bool oneOrSeveralShips,
        BattleShipsConfiguration configuration);

    const QString & enabledAdjacentShips() const { return m_enabledAdjacentShipsString; }
    const QString & oneOrSeveralShips() const { return m_oneOrSeveralShipsString; }
    unsigned int gridWidth() const { return m_battleShipsConfiguration->boardWidth(); }
    unsigned int gridHeight() const { return m_battleShipsConfiguration->boardHeight(); }
    const BattleShipsConfiguration* shipsConfiguration() const { return m_battleShipsConfiguration; }
    void accept(MessageVisitor& visitor) const override;
    static QString messageType() { return QLatin1String("GameOptions"); }
};


class MessageVisitor
{
public:
    virtual ~MessageVisitor() { }
    virtual void visit(const HeaderMessage& msg) = 0;
    virtual void visit(const RejectMessage& msg) = 0;
    virtual void visit(const NickMessage& msg) = 0;
    virtual void visit(const BeginMessage& msg) = 0;
    virtual void visit(const MoveMessage& msg) = 0;
    virtual void visit(const NotificationMessage& msg) = 0;
    virtual void visit(const GameOverMessage& msg) = 0;
    virtual void visit(const RestartMessage& msg) = 0;
    virtual void visit(const ChatMessage& msg) = 0;
    virtual void visit(const GameOptionsMessage& msg) = 0;
};

#endif // MESSAGE_H

