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

KonnectionHandling::KonnectionHandling( QWidget *parent, KBattleshipServer *server ) : QObject( parent )
{
    internalType = KonnectionHandling::SERVER;
    connect( server, SIGNAL( newConnect() ), this, SLOT( serverGotNewClient() ) );
    connect( server, SIGNAL( endConnect() ), this, SLOT( serverLostClient() ) );
    connect( server, SIGNAL( wroteToClient() ), this, SLOT( serverWroteToClient() ) );
    connect( server, SIGNAL( newMessage( KMessage * ) ), this, SLOT( gotNewMessage( KMessage * ) ) );
}

KonnectionHandling::KonnectionHandling( QWidget *parent, KBattleshipClient *client ) : QObject( parent )
{
    internalType = KonnectionHandling::CLIENT;
    connect( client, SIGNAL( endConnect() ), this, SLOT( clientLostServer() ) );
    connect( client, SIGNAL( socketFailure( int ) ), this, SLOT( clientSocketError( int ) ) );
    connect( client, SIGNAL( newMessage( KMessage * ) ), this, SLOT( gotNewMessage( KMessage * ) ) );
}

KonnectionHandling::~KonnectionHandling()
{
}

int KonnectionHandling::getType()
{
    return internalType;
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

void KonnectionHandling::gotBattleFieldState( int fieldx, int fieldy, int state )
{
    KMessageType msgtype;
    msgtype.setType( KMessageType::ANSWER_SHOOT );
    KMessage *msg = new KMessage( msgtype );
    QString qstate;
    qstate.setNum( state );
    msg->addField( QString( "fieldstate" ), qstate );
    msg->addField( QString( "foobar" ), QString( "foostate" ) );
    msg->addField( QString( "foobar1" ), QString( "foostate1" ) );
    kdDebug() << "EMIT!" << endl;
    emit sendMessage( msg );
    emit ownFieldDataChanged( fieldx, fieldy, state );
}

void KonnectionHandling::gotNewMessage( KMessage *msg )
{
    kdDebug() << "GOTNEWMESSAGE!" << endl;
    switch( getType() )
    {
	case KonnectionHandling::CLIENT:
	    switch( msg->getType() )
	    {
		case KMessageType::ENEMY_SHOOT:
		    emit requestBattleFieldState( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt() );
		    break;
		
		case KMessageType::ANSWER_SHOOT:
		    emit enemyFieldDataChanged( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt(), msg->getField( "fieldstate" ).toInt() );
		    break;
	    }
	    break;
    }
}
    
void KonnectionHandling::clientLostServer()
{
    kdDebug() << "ENDSERVER!" << endl;
}

void KonnectionHandling::clientSocketError( int error )
{
    kdDebug() << "CLIENT-ERROR!" << endl;
    switch( error )
    {
	case QSocket::HostLookup:
	    KMessageBox::error( 0L, i18n( "Couldn't lookup host!" ) );
	    emit changeConnectText();
	    break;
	
	default:
	    KMessageBox::error( 0L, i18n( "Unknown Error; No: %1" ).arg( error ) ) ;
	    break;
    }
}

