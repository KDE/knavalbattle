/***************************************************************************
                            kbattleshipserver.cpp
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

#include "kbattleshipserver.moc"

KBattleshipServer::KBattleshipServer( int port ) : QServerSocket( port )
{
    internalPort = port;
}

KBattleshipServer::~KBattleshipServer()
{
}

void KBattleshipServer::start()
{
    if( !ok() )
    {
    	KMessageBox::error( 0L, i18n( "Failed to bind to local port \"%1\"\n\nPlease check if another KBattelship server instance\nis running or another application uses this port." ).arg( internalPort ) ) ;
	emit serverFailure();
    }
}

void KBattleshipServer::newConnection( int socket )
{
    serverSocket = new QSocket( this );
    connect( serverSocket, SIGNAL( readyRead() ), this, SLOT( readClient() ) );
    connect( serverSocket, SIGNAL( delayedCloseFinished() ), this, SLOT( discardClient() ) );
    serverSocket->setSocket( socket );
    emit newConnect();
}

void KBattleshipServer::readClient()
{
    QSocket *socket = ( QSocket * )sender();
    int len = socket->bytesAvailable();
    char *buf = new char[ len + 1 ];
    socket->readBlock( buf, len );
    buf[ len ] = 0;
    KMessage *msg = new KMessage();
    msg->setDataStream( buf );
    emit newMessage( msg );
    delete msg;
    delete[] buf;
}

void KBattleshipServer::sendMessage( KMessage *msg )
{
    QTextStream *post = new QTextStream( serverSocket );
    *post << msg->returnSendStream() << endl;
    emit wroteToClient();
}

void KBattleshipServer::discardClient()
{
    QSocket *socket = ( QSocket * )sender();
    delete socket;
    emit endConnect();
}


