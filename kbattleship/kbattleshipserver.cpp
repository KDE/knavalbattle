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
#include <qtimer.h>
#include <kmessagebox.h>
#include <klocale.h>
#include "kbattleshipserver.moc"

KBattleshipServer::KBattleshipServer(int port) : KExtendedSocket(QString::null, port, inetSocket | passiveSocket)
{
    m_port = port;
    m_serverSocket = 0;
}

KBattleshipServer::~KBattleshipServer()
{
}

void KBattleshipServer::init()
{
    if(listen())
    {
	KMessageBox::error(0L, i18n("Failed to bind to local port \"%1\"\n\nPlease check if another KBattleship server instance\nis running or another application uses this port.").arg(m_port));
	emit sigServerFailure();
        return;
    }
    
    m_connectNotifier = new QSocketNotifier(fd(), QSocketNotifier::Read, this);
    QObject::connect(m_connectNotifier, SIGNAL(activated(int)), SLOT(slotNewConnection()));
}

void KBattleshipServer::slotNewConnection()
{
    KExtendedSocket *sock;
    accept(sock);
    if(sock && m_serverSocket == 0)
    {
	m_serverSocket = sock;
        m_readNotifier = new QSocketNotifier(sock->fd(), QSocketNotifier::Read, this);
	QObject::connect(m_readNotifier, SIGNAL(activated(int)), this, SLOT(slotReadClient()));
	emit sigNewConnect();
    }
    else
        delete sock;
}

void KBattleshipServer::slotDeleteClient()
{
    slotDiscardClient(i18n("The connection broke down!"), false, true);
}

void KBattleshipServer::slotReadClient()
{
    int len;
    ioctl(m_serverSocket->fd(), FIONREAD, &len);
    if(!len)
    {
        QTimer::singleShot(0, this, SLOT(slotDeleteClient()));
        return;
    }

    char *buf = new char[len + 1];
    m_serverSocket->readBlock(buf, len);
    buf[len] = 0;
    KMessage *msg = new KMessage();
    QString buffer = QString::fromUtf8(buf);
    msg->setDataStream(buffer);
    emit sigNewMessage(msg);
    delete []buf;
}

void KBattleshipServer::sendMessage(KMessage *msg)
{
    QCString post = msg->returnSendStream().utf8();
    m_serverSocket->writeBlock(post.data(), post.length());
    emit sigMessageSent(msg);
}

void KBattleshipServer::slotDiscardClient(const QString &reason, bool kmversion, bool bemit)
{
    KMessage *msg = new KMessage(KMessage::DISCARD);
    msg->addField("reason", reason);
    if(kmversion)
	msg->addField("kmversion", "true");
    else
	msg->addField("kmversion", "false");
    QCString post = msg->returnSendStream().utf8();
    m_serverSocket->writeBlock(post.data(), post.length());
    delete msg;
    
    delete m_readNotifier;
    m_readNotifier = 0;
    delete m_serverSocket;
    m_serverSocket = 0;

    if(bemit)
	emit sigEndConnect();
}
