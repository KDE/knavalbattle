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
class Protocol;
class QIODevice;

class NetworkEntity : public Entity, private MessageVisitor
{
Q_OBJECT
    Sea* m_sea;
    Protocol* m_protocol;
    QIODevice* m_socket;
    Shot* m_pending_shot;
    bool m_client;
public:
    NetworkEntity(Sea::Player player, Sea* sea, QIODevice* device, bool client);
    ~NetworkEntity();
    
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    virtual void notifyChat(const Entity* entity, const QString& text);
    virtual void notifyNick(Sea::Player player, const QString& nick);
    virtual void start();
    virtual void startPlaying();
    virtual void notifyReady(Sea::Player player);
    virtual void notifyGameOver(Sea::Player winner);
    virtual void hit(Shot* shot);
    
    virtual KIcon icon() const;
private slots:
    void received(MessagePtr msg);
    virtual void notifyAbort();
protected:
    virtual void visit(const HeaderMessage& msg);
    virtual void visit(const RejectMessage& msg);
    virtual void visit(const NickMessage& msg);
    virtual void visit(const BeginMessage& msg);
    virtual void visit(const MoveMessage& msg);
    virtual void visit(const NotificationMessage& msg);
    virtual void visit(const GameOverMessage& msg);
    virtual void visit(const RestartMessage& msg);
    virtual void visit(const ChatMessage& msg);
signals:
    void restartRequested();
};

#endif // NETWORKENTITY_H

