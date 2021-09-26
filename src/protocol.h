/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBATTLESHIP_PROTOCOL_H
#define KBATTLESHIP_PROTOCOL_H

#include <QString>
#include <QTimer>
#include <QQueue>

#include "message.h"

class QTcpSocket;

class Protocol : public QObject
{
Q_OBJECT
    QTcpSocket* m_device;
    QString m_buffer;
    QQueue<MessagePtr> m_message_queue;
    QTimer m_timer;
    
    MessagePtr parseMessage(const QString& xmlMessage);
public:
    explicit Protocol(QTcpSocket* device);
    
    void send(const MessagePtr& msg);
private Q_SLOTS:
    void readMore();
    void sendNext();
    void processDisconnection();
Q_SIGNALS:
    void received(const MessagePtr&);
    void parseError(const QString&);
    void disconnected();
};

#endif // KBATTLESHIP_PROTOCOL_H
