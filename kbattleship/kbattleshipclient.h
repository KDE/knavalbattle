/***************************************************************************
                             kbattleshipclient.h
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

#ifndef KBATTLESHIPCLIENT_H
#define KBATTLESHIPCLIENT_H

#include <stdlib.h>
#include <kdebug.h>
#include <qsocket.h>
#include <qtextstream.h>
#include "kmessage.h"
#include "kmessagetype.h"

class KBattleshipClient : QSocket
{
    Q_OBJECT
    public:
        KBattleshipClient( QString host = "", int port = 54321 );
        ~KBattleshipClient();

    private slots:
        void connectionControl();
        void lostServer();
        void socketError( int error );

    private:
        void connectToServer();
        int internalPort;
        QString internalHost;
};

#endif

