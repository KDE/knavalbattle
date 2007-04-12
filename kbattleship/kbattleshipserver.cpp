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

#include <config.h>

#include <unistd.h>
#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif
#ifdef HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif
#include <sys/ioctl.h>
#include <qtimer.h>
#include <kmessagebox.h>
#include <klocale.h>
#include "kbattleshipserver.moc"

KBattleshipServer::KBattleshipServer(int port, const QString& name) 
	: KExtendedSocket(QString::null, port, inetSocket | passiveSocket), m_name(name)
{
	m_port = port;
	m_serverSocket = 0;
}

void KBattleshipServer::init()
{
	if(listen())
	{
		KMessageBox::error(0L, i18n("Failed to bind to local port \"%1\"\n\nPlease check if another KBattleship server instance\nis running or another application uses this port.").arg(m_port));
		emit sigServerFailure();
		return;
	}
	m_service.setServiceName(m_name);
	m_service.setType(BATTLESHIP_SERVICE);
	m_service.setPort(m_port);
	m_service.publishAsync();
	m_connectNotifier = new QSocketNotifier(fd(), QSocketNotifier::Read, this);
	QObject::connect(m_connectNotifier, SIGNAL(activated(int)), SLOT(slotNewConnection()));
}

void KBattleshipServer::slotNewConnection()
{
	KExtendedSocket *sock;
	accept(sock);
	if(sock && m_serverSocket == 0)
	{
		m_service.stop();
		m_serverSocket = sock;
		m_readNotifier = new QSocketNotifier(sock->fd(), QSocketNotifier::Read, this);
		QObject::connect(m_readNotifier, SIGNAL(activated(int)), this, SLOT(slotReadClient()));
		emit sigNewConnect();
	}
	else
		delete sock;
}

void KBattleshipServer::slotReadClient()
{
	int len;
	ioctl(m_serverSocket->fd(), FIONREAD, &len);
	if(!len)
	{
		slotDiscardClient(i18n("The connection broke down!"), false, true);
		return;
	}

	char *buf = new char[len + 1];
	m_serverSocket->readBlock(buf, len);
	buf[len] = 0;
	m_readBuffer += QString::fromUtf8(buf);
	delete []buf;
	int pos;
	while ((pos = m_readBuffer.find("</kmessage>")) >= 0) 
	{
		pos += 11; // Length of "</kmessage>"
		KMessage *msg = new KMessage();
		msg->setDataStream(m_readBuffer.left(pos));
		m_readBuffer.remove(0, pos);
		emit sigNewMessage(msg);
	}
}

void KBattleshipServer::sendMessage(KMessage *msg)
{
	QCString post = msg->sendStream().utf8();
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
	QCString post = msg->sendStream().utf8();
	m_serverSocket->writeBlock(post.data(), post.length());
	delete msg;

	delete m_readNotifier;
	m_readNotifier = 0;
	delete m_serverSocket;
	m_serverSocket = 0;

	if(bemit)
		emit sigEndConnect();
}
