/***************************************************************************
                            konnectionhandling.cpp
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

#include "konnectionhandling.moc"

KonnectionHandling::KonnectionHandling( KBattleshipServer *server ) : QObject()
{
    connect( server, SIGNAL( newConnect() ), this, SLOT( serverGotNewClient() ) );
    connect( server, SIGNAL( endConnect() ), this, SLOT( serverLostClient() ) );
    connect( server, SIGNAL( wroteToClient() ), this, SLOT( serverWroteToClient() ) );
}

KonnectionHandling::KonnectionHandling( KBattleshipClient *client ) : QObject()
{
    connect( client, SIGNAL( endConnect() ), this, SLOT( clientLostServer() ) );
    connect( client, SIGNAL( socketFailure( int ) ), this, SLOT( clientSocketError( int ) ) );
}

KonnectionHandling::~KonnectionHandling()
{
}

void KonnectionHandling::serverGotNewClient()
{
    kdDebug() << "NEWCLIENT!" << endl;
}

void KonnectionHandling::serverWroteToClient()
{
    kdDebug() << "WROTETOCLIENT!" << endl;
}

void KonnectionHandling::serverLostClient()
{
    kdDebug() << "ENDCLIENT!" << endl;
}

void KonnectionHandling::clientLostServer()
{
    kdDebug() << "ENDSERVER!" << endl;
}

void KonnectionHandling::clientSocketError( int error )
{
    kdDebug() << "CLIENT-ERROR!" << endl;
}

