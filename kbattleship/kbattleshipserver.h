/***************************************************************************
                             kbattleshipserver.h
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

#ifndef KBATTLESHIPSERVER_H
#define KBATTLESHIPSERVER_H

#include <kdebug.h>
#include <stdlib.h>
#include <qsocket.h>
#include <qserversocket.h>
#include <qtextstream.h>
#include <kmessagebox.h>
#include <klocale.h>
#include "kmessage.h"

class KBattleshipServer : public QServerSocket
{
    Q_OBJECT
    public:
        KBattleshipServer( int port = 54321 );
        ~KBattleshipServer();

        void start();
        void sendMessage( KMessage *msg );
        void allowWrite();
        void forbidWrite();
        bool write();

    private slots:
        void readClient();
        void discardClient();

    signals:
        void senemylist( bool );
        void serverFailure();
        void newConnect();
        void endConnect();
        void wroteToClient();
        void newMessage( KMessage * );
    
    private:
        int internalPort;
        QSocket *serverSocket;
        void newConnection( int socket );
        bool writeable;
};

#endif

