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

KBattleshipClient::KBattleshipClient( QString host, int port ) : QSocket()
{
    internalHost = host;
    internalPort = port;
    connect( this, SIGNAL( error( int ) ), SLOT( socketError( int ) ) );

    connect( this, SIGNAL( hostFound() ), SLOT( connectionControl() ) );
    connect( this, SIGNAL( connected() ), SLOT( connectionControl() ) );
    connect( this, SIGNAL( connectionClosed() ), SLOT( connectionControl() ) );
    connect( this, SIGNAL( readyRead() ), SLOT( connectionControl() ) );

    connectToServer();
}

KBattleshipClient::~KBattleshipClient()
{
}

void KBattleshipClient::connectToServer()
{
    connectToHost( internalHost, internalPort);
}

void KBattleshipClient::connectionControl()
{
    kdDebug() << "Client-State: " << state() << endl;
}

void KBattleshipClient::lostServer()
{
}

void KBattleshipClient::socketError( int error )
{
}
