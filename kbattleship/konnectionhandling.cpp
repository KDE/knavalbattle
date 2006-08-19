/**************************************MM*************************************
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

extern const char *protocolVersion;

KonnectionHandling::KonnectionHandling(QWidget *parent, KBattleshipServer *server) : QObject(parent)
{
	m_kbserver = server;
	m_kbclient = 0;
	m_type = KonnectionHandling::SERVER;
	connect(server, SIGNAL(sigServerFailure()), this, SIGNAL(sigAbortNetworkGame()));
	connect(server, SIGNAL(sigNewConnect()), this, SLOT(slotNewClient()));
	connect(server, SIGNAL(sigEndConnect()), this, SLOT(slotLostClient()));
	connect(server, SIGNAL(sigNewMessage(KMessage *)), this, SLOT(slotNewMessage(KMessage *)));
	connect(server, SIGNAL(sigMessageSent(KMessage *)), this, SLOT(slotMessageSent(KMessage *)));
}

KonnectionHandling::KonnectionHandling(QWidget *parent, KBattleshipClient *client) : QObject(parent)
{
	m_kbclient = client;
	m_kbserver = 0;
	m_type = KonnectionHandling::CLIENT;
	connect(client, SIGNAL(sigEndConnect()), this, SLOT(slotLostServer()));
	connect(client, SIGNAL(sigSocketFailure(KNetwork::KStreamSocket::SocketError)), this, SLOT(slotSocketError(KNetwork::KStreamSocket::SocketError)));
	connect(client, SIGNAL(sigNewMessage(KMessage *)), this, SLOT(slotNewMessage(KMessage *)));
	connect(client, SIGNAL(sigMessageSent(KMessage *)), this, SLOT(slotMessageSent(KMessage *)));
}

void KonnectionHandling::updateInternal(KBattleshipServer *server)
{
	m_kbserver = server;
	m_kbclient = 0;
	m_type = KonnectionHandling::SERVER;
	connect(server, SIGNAL(sigServerFailure()), this, SIGNAL(sigAbortNetworkGame()));
	connect(server, SIGNAL(sigNewConnect()), this, SLOT(slotNewClient()));
	connect(server, SIGNAL(sigEndConnect()), this, SLOT(slotLostClient()));
	connect(server, SIGNAL(sigNewMessage(KMessage *)), this, SLOT(slotNewMessage(KMessage *)));
	connect(server, SIGNAL(sigMessageSent(KMessage *)), this, SLOT(slotMessageSent(KMessage *)));
}

void KonnectionHandling::updateInternal(KBattleshipClient *client)
{
	m_kbclient = client;
	m_kbserver = 0;
	m_type = KonnectionHandling::CLIENT;
	connect(client, SIGNAL(sigEndConnect()), this, SLOT(slotLostServer()));
	connect(client, SIGNAL(sigSocketFailure(KNetwork::KStreamSocket::SocketError)), this, SLOT(slotSocketError(KNetwork::KStreamSocket::SocketError)));
	connect(client, SIGNAL(sigNewMessage(KMessage *)), this, SLOT(slotNewMessage(KMessage *)));
	connect(client, SIGNAL(sigMessageSent(KMessage *)), this, SLOT(slotMessageSent(KMessage *)));
}

void KonnectionHandling::slotNewClient()
{
}

void KonnectionHandling::slotLostClient()
{
	KMessageBox::error(0L, i18n("Connection to client lost. Aborting the game."));
	emit sigClientLost();
}

void KonnectionHandling::slotMessageSent(KMessage *msg)
{
	if(msg->type() == KMessage::SHOOT)
		emit sigShootable(false);

	delete msg;
}

void KonnectionHandling::slotNewMessage(KMessage *msg)
{
	KMessage *copy;
	if(type() == KonnectionHandling::CLIENT)
	{
		switch(msg->type())
		{
			// First possible message
			case KMessage::DISCARD:
				if(msg->field("kmversion") == QString("true"))
				{
					KMessageBox::error(0L, i18n("Connection dropped by enemy. The client's protocol implementation (%1) is not compatible with our (%2) version.", msg->field("reason"), protocolVersion));
					emit sigAbortNetworkGame();
				}
				else
					KMessageBox::error(0L, i18n(msg->field("reason").toLatin1()));
				break;

				// Got some information
			case KMessage::GETVERSION:
				emit sigClientInformation(msg->field("clientName"), msg->field("clientVersion"), msg->field("clientDescription"), msg->field("protocolVersion"));
				break;

				// Got the enemy's nickname
			case KMessage::GREET:
				emit sigEnemyNickname(msg->field("nickname"));
				emit sigStatusBar(i18n("Waiting for other player to place their ships..."));
				break;

				// The server wants ous to place the ships
			case KMessage::SHIPSREADY:
				emit sigPlaceShips(true);
				emit sigStatusBar(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
				break;

				// The server shot and wants the field state
			case KMessage::SHOOT:
				emit sigSendFieldState(msg->field("fieldx").toInt(), msg->field("fieldy").toInt());
				emit sigStatusBar(i18n("Enemy has shot. Shoot now."));
				emit sigShootable(true);
				break;

				// The server gave us the field data of our last shot
			case KMessage::ANSWER_SHOOT:
				emit sigShootable(false);
				emit sigEnemyFieldData(msg->field("fieldx").toInt(), msg->field("fieldy").toInt(), msg->field("fieldstate").toInt(), msg->field("xstart").toInt(), msg->field("xstop").toInt(), msg->field("ystart").toInt(), msg->field("ystop").toInt(), (msg->field("death") == QString("true")));
				break;

				// The server starts a new game
			case KMessage::REPLAY:
				emit sigStatusBar(i18n("Waiting for other player to place their ships..."));
				emit sigReplay();
				break;

				// We lost the game
			case KMessage::WON:
				emit sigStatusBar(i18n("You lost the game :("));
				copy = new KMessage(msg);
				emit sigLost(copy);
				break;

				// We got a chat message
			case KMessage::CHAT:
				emit sigChatMessage(msg->field("nickname"), msg->field("chat"), true);
				break;
		}
	}
	else
	{
		switch(msg->type())
		{
			// First message....got client information
			case KMessage::GETVERSION:
				if(msg->field("protocolVersion") != QString::fromLatin1(protocolVersion))
				{
					m_kbserver->slotDiscardClient(protocolVersion, true, false);
					KMessageBox::error(0L, i18n("Connection to client dropped. The client's protocol implementation (%1) is not compatible with our (%2) version.", msg->field("protocolVersion"), protocolVersion));
				}
				else
					emit sigClientInformation(msg->field("clientName"), msg->field("clientVersion"), msg->field("clientDescription"), msg->field("protocolVersion"));
				break;

				// Got the enemy's nickname
			case KMessage::GREET:
				KMessageBox::information(0L, i18n("We have a player. Let's start..."));
				emit sigStatusBar(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
				emit sigEnemyNickname(msg->field("nickname"));
				emit sigSendNickname();
				emit sigPlaceShips(true);
				break;

				// The client placed his ships...we can shoot now
			case KMessage::SHIPSREADY:
				emit sigShootable(true);
				emit sigStatusBar(i18n("You can shoot now."));
				break;

				// The client gave us the field data of our last shot
			case KMessage::ANSWER_SHOOT:
				emit sigShootable(false);
				emit sigEnemyFieldData(msg->field("fieldx").toInt(), msg->field("fieldy").toInt(), msg->field("fieldstate").toInt(), msg->field("xstart").toInt(), msg->field("xstop").toInt(), msg->field("ystart").toInt(), msg->field("ystop").toInt(), (msg->field("death") == QString("true")));
				break;

				// The client shot and wants the field state
			case KMessage::SHOOT:
				emit sigSendFieldState(msg->field("fieldx").toInt(), msg->field("fieldy").toInt());
				emit sigStatusBar(i18n("Enemy has shot. Shoot now."));
				emit sigShootable(true);
				break;

				// The client asks for a replay
			case KMessage::REPLAY:
				emit sigReplay();
				break;

				// We lost the game
			case KMessage::WON:
				emit sigStatusBar(i18n("You lost the game :("));
				copy = new KMessage(msg);
				emit sigLost(copy);
				break;

				// We got a chat message
			case KMessage::CHAT:
				emit sigChatMessage(msg->field("nickname"), msg->field("chat"), true);
				break;
		}
	}

	delete msg;
}

void KonnectionHandling::slotSocketError(KNetwork::KStreamSocket::SocketError error)
{
	switch(error)
	{
		case KNetwork::KStreamSocket::ConnectionRefused:
			KMessageBox::error(0L, i18n("Connection refused by other host."));
			break;

		case KNetwork::KStreamSocket::LookupFailure:
			KMessageBox::error(0L, i18n("Couldn't lookup host."));
			break;

		default:
			KMessageBox::error(0L, i18n("Unknown error; No: %1", error));
			break;
	}

	emit sigAbortNetworkGame();
}

void KonnectionHandling::slotLostServer()
{
	m_kbclient->close();
	m_kbclient->deleteLater();
	KMessageBox::error(0L, i18n("Connection to server lost. Aborting the game."));
	emit sigServerLost();
}
