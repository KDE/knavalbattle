/***************************************************************************
                            konnectionhandling.cpp
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

#include "konnectionhandling.moc"

KonnectionHandling::KonnectionHandling(QWidget *parent, KBattleshipServer *server) : QObject(parent)
{
    m_showed = false;
    enemy = false;
    setEnemyList(false);
    enemylist = false;
    internalServer = server;
    internalClient = 0;
    internalType = KonnectionHandling::SERVER;
    connect(server, SIGNAL(serverFailure()), this, SLOT(serverError()));
    connect(server, SIGNAL(senemylist(bool)), this, SLOT(setEnemyList(bool)));
    connect(server, SIGNAL(newConnect()), this, SLOT(serverGotNewClient()));
    connect(server, SIGNAL(endConnect()), this, SLOT(serverLostClient()));
    connect(server, SIGNAL(wroteToClient()), this, SLOT(serverWroteToClient()));
    connect(server, SIGNAL(newMessage(KMessage *)), this, SLOT(gotNewMessage(KMessage *)));
}

KonnectionHandling::KonnectionHandling(QWidget *parent, KBattleshipClient *client) : QObject(parent)
{
    m_showed = false;
    enemy = true;
    setEnemyList(false);
    enemylist = false;
    internalClient = client;
    internalServer = 0;
    internalType = KonnectionHandling::CLIENT;
    connect(client, SIGNAL(senemylist(bool)), this, SLOT(setEnemyList(bool)));
    connect(client, SIGNAL(endConnect()), this, SLOT(clientLostServer()));
    connect(client, SIGNAL(socketFailure(int)), this, SLOT(clientSocketError(int)));
    connect(client, SIGNAL(newMessage(KMessage *)), this, SLOT(gotNewMessage(KMessage *)));
}

KonnectionHandling::~KonnectionHandling()
{
}

void KonnectionHandling::updateInternal(KBattleshipServer *server)
{
    m_showed = false;
    enemy = false;
    setEnemyList(false);
    enemylist = false;
    internalServer = server;
    internalClient = 0;
    internalType = KonnectionHandling::SERVER;
    connect(server, SIGNAL(serverFailure()), this, SLOT(serverError()));
    connect(server, SIGNAL(senemylist(bool)), this, SLOT(setEnemyList(bool)));
    connect(server, SIGNAL(newConnect()), this, SLOT(serverGotNewClient()));
    connect(server, SIGNAL(endConnect()), this, SLOT(serverLostClient()));
    connect(server, SIGNAL(wroteToClient()), this, SLOT(serverWroteToClient()));
    connect(server, SIGNAL(newMessage(KMessage *)), this, SLOT(gotNewMessage(KMessage *)));
}

void KonnectionHandling::updateInternal(KBattleshipClient *client)
{
    m_showed = false;
    enemy = true;
    setEnemyList(false);
    enemylist = false;
    internalClient = client;
    internalServer = 0;
    internalType = KonnectionHandling::CLIENT;
    connect(client, SIGNAL(senemylist(bool)), this, SLOT(setEnemyList(bool)));
    connect(client, SIGNAL(endConnect()), this, SLOT(clientLostServer()));
    connect(client, SIGNAL(socketFailure(int)), this, SLOT(clientSocketError(int)));
    connect(client, SIGNAL(newMessage(KMessage *)), this, SLOT(gotNewMessage(KMessage *)));
}

void KonnectionHandling::clear()
{
    enemy = true;
    setEnemyList(false);
    enemylist = false;
    if(internalClient != 0)
	internalClient->allowWrite();
    if(internalServer != 0)
	internalServer->allowWrite();
}

int KonnectionHandling::getType()
{
    return internalType;
}

void KonnectionHandling::serverError()
{
    emit serverFailure(true);
}

bool KonnectionHandling::haveEnemy()
{
    return enemy;
}

bool KonnectionHandling::gotEnemyList()
{
    return enemylist;
}

void KonnectionHandling::setEnemyList(bool set)
{
    senemylist = set;
}

bool KonnectionHandling::sendEnemyList()
{
    return senemylist;
}

void KonnectionHandling::serverGotNewClient()
{
    enemy = true;
    KMessageBox::information(0L, i18n("We got a player. Let's start..."));
    emit newPlayer(true);
    emit statusBarMessage(i18n("Please place your ships"));
}

void KonnectionHandling::serverWroteToClient()
{
}

void KonnectionHandling::serverLostClient()
{
    KMessageBox::error(0L, i18n("Connection to client lost. Aborting the game!"));
    emit abortGameStrict(true);
}

bool KonnectionHandling::writeable()
{
    if(getType() == KonnectionHandling::CLIENT)
        return internalClient->write();
    else if(getType() == KonnectionHandling::SERVER)
        return internalServer->write();
    return false;
}

void KonnectionHandling::gotNewMessage(KMessage *msg)
{
    if(getType() == KonnectionHandling::CLIENT)
    {
	switch(msg->getType())
	{
	    case KMessage::REPLAY:
		emit clientRestart();
		break;
		    
	    case KMessage::GREET:
	        emit enemyNickname(msg->getField("nickname"));
    	        emit newPlayer(true);
	        break;
		    
	    case KMessage::SHIPLIST:
	        enemylist = true;
	        emit gotEnemyShipList(msg->getField("fieldx1s1"), msg->getField("fieldy1s1"), msg->getField("fieldx2s1"), msg->getField("fieldy2s1"), msg->getField("fieldx1s2"), msg->getField("fieldy1s2"), msg->getField("fieldx2s2"), msg->getField("fieldy2s2"), msg->getField("fieldx1s3"), msg->getField("fieldy1s3"), msg->getField("fieldx2s3"), msg->getField("fieldy2s3"), msg->getField("fieldx1s4"), msg->getField("fieldy2s4"), msg->getField("fieldx2s4"), msg->getField("fieldy2s4"));
		emit setPlaceable();
		emit statusBarMessage(i18n("Please place your ships"));
		break;
			
	    case KMessage::ANSWER_SHOOT:
	        emit ownFieldDataChanged(msg->getField("fieldx").toInt(), msg->getField("fieldy").toInt(), msg->getField("fieldstate").toInt());
	        if(msg->enemyWon())
	        {
		    emit statusBarMessage(i18n("You lost the game :("));
		    emit updateHighscore();
		    emit abortGame(true);
		}
		else
		    emit statusBarMessage(i18n("Enemy has shot. Shoot now"));
	        break;
		    
	    case KMessage::CHAT:
	        emit gotChatMessage(msg->getField("nickname"), msg->getField("chat"), true);
	        break;
        }
        if(msg->enemyReady())
	    internalClient->allowWrite();
    }
    else
    {
        switch(msg->getType())
        {
	    case KMessage::REPLAY:
	        emit askReplay();
	        break;
		    
	    case KMessage::GREET:
	        emit enemyNickname(msg->getField("nickname"));
	        emit giveEnemyName();
	        break;
		    
	    case KMessage::SHIPLIST:
	        enemylist = true;
	        emit gotEnemyShipList(msg->getField("fieldx1s1"), msg->getField("fieldy1s1"), msg->getField("fieldx2s1"), msg->getField("fieldy2s1"), msg->getField("fieldx1s2"), msg->getField("fieldy1s2"), msg->getField("fieldx2s2"), msg->getField("fieldy2s2"), msg->getField("fieldx1s3"), msg->getField("fieldy1s3"), msg->getField("fieldx2s3"), msg->getField("fieldy2s3"), msg->getField("fieldx1s4"), msg->getField("fieldy2s4"), msg->getField("fieldx2s4"), msg->getField("fieldy2s4"));
	        emit statusBarMessage(i18n("You can shoot now"));
	        break;
		    
	    case KMessage::ANSWER_SHOOT:
	        emit ownFieldDataChanged(msg->getField("fieldx").toInt(), msg->getField("fieldy").toInt(), msg->getField("fieldstate").toInt());
	        if(msg->enemyWon())
	        {
	            emit statusBarMessage(i18n("You lost the game :("));
		    emit updateHighscore();
	            emit abortGame(true);
	        }
	        else
	            emit statusBarMessage(i18n("Enemy has shot. Shoot now"));
	            break;
		    
	    case KMessage::CHAT:
	        emit gotChatMessage(msg->getField("nickname"), msg->getField("chat"), true);
	        break;
        }
        if(msg->enemyReady())
	    internalServer->allowWrite();
    }
}
    
void KonnectionHandling::clientLostServer()
{
    KMessageBox::error(0L, i18n("Connection to server lost. Aborting the game!"));
    emit abortGameStrict(true);
}

void KonnectionHandling::clientSocketError(int error)
{
    if(!m_showed)
    {
	m_showed = true;
	switch(error)
	{
    	    case QSocket::ErrConnectionRefused:
 		KMessageBox::error(0L, i18n("Connection refused by other host!"));
        	break;

	    case QSocket::ErrHostNotFound:
		KMessageBox::error(0L, i18n("Couldn't lookup host!"));
		break;
	
	    case QSocket::ErrSocketRead:	
 	        KMessageBox::error(0L, i18n("Couldn't connect to server!"));
    	        break;
	
    	    default:
		KMessageBox::error(0L, i18n("Unknown Error; No: %1").arg(error));
		break;
	}
    }
    emit changeConnectText();
}
