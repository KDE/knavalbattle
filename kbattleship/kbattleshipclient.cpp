/***************************************************************************
                            kbattleshipclient.cpp
                             -------------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <sys/ioctl.h>
#include <qsocketnotifier.h>
#include "kmessage.h"
#include "kbattleshipclient.moc"

KBattleshipClient::KBattleshipClient(const QString &host, int port) : KExtendedSocket(host, port, inetSocket)
{
}

KBattleshipClient::~KBattleshipClient()
{
}

void KBattleshipClient::init()
{
    if(connect())
    {
	emit sigSocketFailure(status());
        return;
    }
    
    m_readNotifier = new QSocketNotifier(fd(), QSocketNotifier::Read, this);
    QObject::connect(m_readNotifier, SIGNAL(activated(int)), SLOT(slotReadData()));
    emit sigConnected();
}

void KBattleshipClient::sendMessage(KMessage *msg)
{
    QCString post = msg->returnSendStream().utf8();
    writeBlock(post.data(), post.length());
    emit sigMessageSent(msg);
}

void KBattleshipClient::slotReadData()
{
    int len;
    ioctl(fd(), FIONREAD, &len);
    if(!len)
    {
        delete m_readNotifier;
        m_readNotifier = 0;
        emit sigEndConnect();
        return;
    }

    char *buf = new char[len + 1];
    readBlock(buf, len);
    buf[len] = 0;
    KMessage *msg = new KMessage();
    QString buffer = QString::fromUtf8(buf);
    msg->setDataStream(buffer);
    emit sigNewMessage(msg);
    delete []buf;
}
