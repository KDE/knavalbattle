/***************************************************************************
                             kbattleshipserver.h
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

#ifndef KBATTLESHIPSERVER_H
#define KBATTLESHIPSERVER_H

#include <kextsock.h>
#include <qsocketnotifier.h>
#include "kmessage.h"

class KBattleshipServer : public KExtendedSocket
{
    Q_OBJECT
    public:
        KBattleshipServer(int port);
        ~KBattleshipServer();

        void init();
        void sendMessage(KMessage *msg);

    public slots:
	void slotDiscardClient(const QString &reason, bool kmversion, bool bemit);

    private slots:
        void slotNewConnection();
        void slotReadClient();
	void slotDeleteClient();

    signals:
        void sigServerFailure();
        void sigNewConnect();
        void sigEndConnect();
        void sigNewMessage(KMessage *);
	void sigMessageSent(KMessage *);
    
    private:
        QSocketNotifier *m_connectNotifier;
        QSocketNotifier *m_readNotifier;
        KExtendedSocket *m_serverSocket;
        QString m_readBuffer;
        int m_port;
};

#endif
