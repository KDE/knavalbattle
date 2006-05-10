/***************************************************************************
                                 konnectionhandling.h
                                  -----------------
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

#ifndef KONNECTIONHANDLING_H
#define KONNECTIONHANDLING_H

#include <klocale.h>
#include <kmessagebox.h>

#include <QObject>

#include "kbattleshipclient.h"
#include "kbattleshipserver.h"
#include "kmessage.h"

class KonnectionHandling : public QObject
{
	Q_OBJECT
public:
	enum{SERVER, CLIENT};
	KonnectionHandling(QWidget *parent, KBattleshipServer *server);
	KonnectionHandling(QWidget *parent, KBattleshipClient *client);

	int type() { return m_type; }

	void updateInternal(KBattleshipServer *server);
	void updateInternal(KBattleshipClient *client);

public slots:
	void slotNewMessage(KMessage *msg);
	void slotMessageSent(KMessage *msg);
	void slotNewClient();
	void slotLostClient();
	void slotLostServer();
	void slotSocketError(KNetwork::KStreamSocket::SocketError error);

signals:
	void sigStatusBar(const QString &);
	void sigEnemyNickname(const QString &);
	void sigEnemyFieldData(int, int, int, int, int, int, int, bool);
	void sigClientInformation(const QString &, const QString &, const QString &, const QString &);
	void sigSendNickname();
	void sigSendFieldState(int, int);
	void sigPlaceShips(bool);
	void sigShootable(bool);
	void sigClientLost();
	void sigServerLost();
	void sigReplay();
	void sigLost(KMessage *);
	void sigAbortNetworkGame();
	void sigChatMessage(const QString &, const QString &, bool);

private:
	KBattleshipServer *m_kbserver;
	KBattleshipClient *m_kbclient;
	int m_type;
};

#endif
