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
    allowWrite();
}

KBattleshipClient::~KBattleshipClient()
{
}

void KBattleshipClient::init()
{
    if(connect())
    {
	emit socketFailure(status());
        return;
    }
    
    readNotifier = new QSocketNotifier(fd(), QSocketNotifier::Read, this);
    QObject::connect(readNotifier, SIGNAL(activated(int)), SLOT(readData()));
    emit connected();
}

void KBattleshipClient::sendMessage(KMessage *msg)
{
    if(writeable)
    {
	QCString post = msg->returnSendStream().utf8();
        writeBlock(post.data(), post.length());
	if(msg->enemyReady())
	{
	    forbidWrite();
	    emit senemylist(true);
	}
	delete msg;
    }
}

void KBattleshipClient::readData()
{
    int len;
    ioctl(fd(), FIONREAD, &len);
    if(!len)
    {
        delete readNotifier;
        readNotifier = 0;
        emit endConnect();
        return;
    }
    char *buf = new char[len + 1];
    readBlock(buf, len);
    buf[len] = 0;
    KMessage *msg = new KMessage();
    QString buffer = QString::fromUtf8(buf);
    msg->setDataStream(buffer);
    emit newMessage(msg);
    delete msg;
    delete []buf;
}
