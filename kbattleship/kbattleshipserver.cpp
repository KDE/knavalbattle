/***************************************************************************
                            kbattleshipserver.cpp
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
#include <qtimer.h>

#include <klocale.h>
#include <kmessagebox.h>

#include "kmessage.h"
#include "kbattleshipserver.moc"

KBattleshipServer::KBattleshipServer(int port)
    : KExtendedSocket(QString::null, port, inetSocket | passiveSocket)
{
    internalPort = port;
    serverSocket = 0;

    allowWrite();
}

KBattleshipServer::~KBattleshipServer()
{
}

void KBattleshipServer::start()
{
    if(listen())
    {
	KMessageBox::error(0L, i18n("Failed to bind to local port \"%1\"\n\nPlease check if another KBattleship server instance\nis running or another application uses this port.").arg(internalPort));
	emit serverFailure();
        return;
    }
    int reuse = 1;
    connectNotifier = new QSocketNotifier(fd(), QSocketNotifier::Read, this);
    QObject::connect(connectNotifier, SIGNAL(activated(int)), SLOT(newConnection()));
}

void KBattleshipServer::newConnection()
{
    KExtendedSocket *sock;
    accept(sock);
    if(sock && serverSocket == 0)
    {
	serverSocket = sock;
        readNotifier = new QSocketNotifier(sock->fd(), QSocketNotifier::Read, this);
	QObject::connect(readNotifier, SIGNAL(activated(int)), this, SLOT(readClient()));
	emit newConnect();
    }
    else
        delete sock; // Got a connection already
}

void KBattleshipServer::readClient()
{
    int len;
    ioctl(serverSocket->fd(), FIONREAD, &len);
    if (!len)
    {
        QTimer::singleShot(0, this, SLOT(discardClient()));
        return;
    }
    char *buf = new char[len + 1];
    serverSocket->readBlock(buf, len);
    buf[len] = 0;
    KMessage *msg = new KMessage();
    QString buffer = QString::fromUtf8(buf);
    msg->setDataStream(buffer);
    emit newMessage(msg);
    delete msg;
    delete []buf;
}

void KBattleshipServer::sendMessage(KMessage *msg)
{
    if(writeable)
    {
	QCString post = msg->returnSendStream().utf8();
        serverSocket->writeBlock(post.data(), post.length());
	emit wroteToClient();
	if(msg->enemyReady())
	{
	    forbidWrite();
	    emit senemylist(true);
	}
	delete msg;
    }
}

void KBattleshipServer::discardClient()
{
    delete readNotifier;
    readNotifier = 0;
    delete serverSocket;
    serverSocket = 0;
    emit endConnect();
}
