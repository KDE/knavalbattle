/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef NETWORKENTITY_H
#define NETWORKENTITY_H

#include "entity.h"
#include "message.h"

class Sea;
class SeaView;
class Protocol;

class NetworkEntity : public Entity, private MessageVisitor
{
Q_OBJECT
    Sea* m_sea;
    Protocol* m_protocol;
    Shot* m_pending_shot;
    bool m_client;
    bool m_winner;
    
public:
    NetworkEntity(Sea::Player player, Sea* sea, SeaView* seaview, Protocol* device, bool client);
    ~NetworkEntity() override;

    void notify(Sea::Player player, const Coord& c, const HitInfo& info) override;
    void notifyChat(const Entity* entity, const QString& text) override;
    void notifyNick(Sea::Player player, const QString& nick) override;
    void start() override;
    void startPlacing() override;
    void startPlaying() override;
    void notifyReady(Sea::Player player) override;
    void notifyShips(Sea::Player winner) override;
    void notifyGameOver(Sea::Player winner) override;
    void notifyGameOptions() override;
    void notifyRestart(Sea::Player) override;
    void hit(Shot* shot) override;

    QIcon icon() const override;
private slots:
    void received(MessagePtr msg);
    void notifyAbort() override;
    void notifyRestartPlacing(Sea::Player) override { }
protected:
    void visit(const HeaderMessage& msg) override;
    void visit(const RejectMessage& msg) override;
    void visit(const NickMessage& msg) override;
    void visit(const BeginMessage& msg) override;
    void visit(const MoveMessage& msg) override;
    void visit(const NotificationMessage& msg) override;
    void visit(const GameOverMessage& msg) override;
    void visit(const RestartMessage& msg) override;
    void visit(const ChatMessage& msg) override;
    void visit(const GameOptionsMessage& msg) override;
signals:
    void restartRequested();
};

#endif // NETWORKENTITY_H

