/***************************************************************************
                             kbattleshipclient.h
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

#ifndef KBATTLESHIPCLIENT_H
#define KBATTLESHIPCLIENT_H

#include <kextsock.h>
#include <qsocketnotifier.h>
#include "kmessage.h"

class KBattleshipClient : public KExtendedSocket
{
	Q_OBJECT
public:
	KBattleshipClient(const QString &host, int port);

	void init();
	void sendMessage(KMessage *msg);

private slots:
	void slotReadData();

signals:
	void sigConnected();
	void sigEndConnect();
	void sigSocketFailure(int);
	void sigNewMessage(KMessage *);
	void sigMessageSent(KMessage *);

private:
	QSocketNotifier *m_readNotifier;
	QString m_readBuffer;
};

#endif
