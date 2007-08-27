/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef KBATTLESHIP_PROTOCOL_H
#define KBATTLESHIP_PROTOCOL_H

#include <QString>
#include <QTimer>
#include <QQueue>
#include "message.h"

class QIODevice;

class Protocol : public QObject
{
Q_OBJECT
    QIODevice* m_device;
    QString m_buffer;
    QQueue<MessagePtr> m_message_queue;
    QTimer m_timer;
    
    MessagePtr parseMessage(const QString& xmlMessage);
public:
    explicit Protocol(QIODevice* device);
    
    void send(const MessagePtr& msg);
private slots:
    void readMore();
    void sendNext();
    void processDisconnection();
signals:
    void received(MessagePtr);
    void parseError(const QString&);
    void disconnected();
};

#endif // KBATTLESHIP_PROTOCOL_H
