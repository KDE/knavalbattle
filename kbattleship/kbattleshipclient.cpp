/***************************************************************************
                            kbattleshipclient.cpp
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kbattleshipclient.moc"

KBattleshipClient::KBattleshipClient(QString host, int port) : QSocket()
{
    internalHost = host;
    internalPort = port;
    connect(this, SIGNAL(error(int)), this, SLOT(socketError(int)));
    connect(this, SIGNAL(hostFound()), this, SLOT(connectionControl()));
    connect(this, SIGNAL(connectionClosed()), this, SLOT(lostServer()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));

    allowWrite();
}

KBattleshipClient::~KBattleshipClient()
{
}

void KBattleshipClient::init()
{
    connectToHost(internalHost, internalPort);
}

void KBattleshipClient::kill()
{
    close();
}

void KBattleshipClient::sendMessage(KMessage *msg)
{
    if(writeable)
    {
	QTextStream post(this);
	post << msg->returnSendStream();
	if(msg->enemyReady())
	{
	    forbidWrite();
	    emit senemylist(true);
	}
	delete msg;
    }
}

void KBattleshipClient::connectionControl()
{
    kdDebug() << "Client-State: " << state() << endl;
}

void KBattleshipClient::readData()
{
    if(canReadLine())
    {
	KMessage *msg = new KMessage();
	msg->setDataStream(readLine());
	emit newMessage(msg);
	delete msg;
    }
}

void KBattleshipClient::lostServer()
{
    emit endConnect();
}

void KBattleshipClient::socketError(int error)
{
    emit socketFailure(error);
}
