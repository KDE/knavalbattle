/***************************************************************************
                             kbattleshipserver.h
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <nikoz@gymnasium-kerpen.de>
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
#define KBATTLESHIPTSERVER_H

#include <kdebug.h>
#include <qsocket.h>
#include <qserversocket.h>
#include "kmessage.h"
#include "kmessagetype.h"

class KBattleshipServer : QServerSocket
{
    Q_OBJECT
    public:
        KBattleshipServer( int port );
        ~KBattleshipServer();

    protected:
        void newConnection();
        void discardClient();

    signals:
        void newConnect();
        void endConnect();
        void wroteToClient();
};

#endif

