/***************************************************************************
                            kbattleshipserver.cpp
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

#include "kbattleshipserver.moc"

KBattleshipServer::KBattleshipServer( int port ) : QServerSocket( int port = 54321 )
{
    if( !ok )
    {
     kdDebug() << "Failed to bind on Port " << port << " !" << endl;
     exit(1);
    }
}

KBattleshipServer::~KBattleshipServer()
{
}

void KBattleshipServer::newConnection( int socket )
{
    QSocket *serverSocket = new QSocket( this );
    connect( serverSocket, SIGNAL( readyRead() ), this, SLOT( readClient() ) );
    connect( serverSocket, SIGNAL( delayedCloseFinished() ), this, SLOT( discardClient() ) );
    serverSocket->setSocket( socket );
    emit newConnect();
}

void KBattleshipServer::readClient()
{
    QSocket* socket = (QSocket*)sender();
    if( socket->canReadLine() )
    {
        KMessageType msgtype;
        KMessage( msgtype ) message;
        message.setDataStream( socket->readLine() );
        kdDebug() << "Type of message: " << message.getType() << endl;
    }
}

void KBattleshipServer::sendMessage( KMessage &msg )
{
    QSocket* socket = (QSocket*)sender();
    QTextStream post( socket );
    socket << msg.returnSendStream;
    emit wroteToClient();
}

void discardClient()
{
    QSocket* socket = (QSocket*)sender();
    QTextStream post( socket );
    KMessage msg;
    KMessageType msgtype;
    msgtype.setType( KMessageType::MSG_FORBIDDEN );
    msgtype.addField( QString( "forbidden" ), QString( "add a reason here!" ) );
    socket << msg.returnSendStream();
    emit wroteToClient();
    delete socket;
    emit endConnect();
}


