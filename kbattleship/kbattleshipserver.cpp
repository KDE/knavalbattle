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
    if( !ok() )
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
    serverSocket = new QSocket( this );
    connect( serverSocket, SIGNAL( readyRead() ), this, SLOT( readClient() ) );
    connect( serverSocket, SIGNAL( delayedCloseFinished() ), this, SLOT( discardClient() ) );
    serverSocket->setSocket( socket );
    emit newConnect();
}

void KBattleshipServer::readClient()
{
    QSocket *socket = ( QSocket * )sender();
    kdDebug() << "READLINE!" << endl;
    if( socket->canReadLine() )
    {
	kdDebug() << "CAN READ!" << endl;
        KMessageType msgtype;
        KMessage *msg = new KMessage( msgtype );
        msg->setDataStream( socket->readLine() );
//	kdDebug() << "msg: " << serverSocket->readLine() << endl;
	kdDebug() << "Type of message: " << msg->getType() << endl;
	emit newMessage( msg );
    }
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
    //QTextStream post( socket );
    //KMessageType msgtype;
    //msgtype.setType( KMessageType::MSG_FORBIDDEN );
    //KMessage *msg=new KMessage( msgtype );
    //msg->addField( QString( "forbidden" ), QString( "didnotaccept" ) );
    ////post << msg->returnSendStream();
    //emit wroteToClient();
    delete socket;
    emit endConnect();
}


