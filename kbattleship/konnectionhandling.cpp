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
    enemy = false;
    setEnemyList( false );
    enemylist = false;
    internalServer = server;
    internalType = KonnectionHandling::SERVER;
    connect( server, SIGNAL( serverFailure() ), this, SIGNAL( serverFailure() ) );
    connect( server, SIGNAL( senemylist( bool ) ), this, SLOT( setEnemyList( bool ) ) );
    connect( server, SIGNAL( newConnect() ), this, SLOT( serverGotNewClient() ) );
    connect( server, SIGNAL( endConnect() ), this, SLOT( serverLostClient() ) );
    connect( server, SIGNAL( wroteToClient() ), this, SLOT( serverWroteToClient() ) );
    connect( server, SIGNAL( newMessage( KMessage * ) ), this, SLOT( gotNewMessage( KMessage * ) ) );
}

KonnectionHandling::KonnectionHandling( QWidget *parent, KBattleshipClient *client ) : QObject( parent )
{
    enemy = true;
    setEnemyList( false );
    enemylist = false;
    internalClient = client;
    internalType = KonnectionHandling::CLIENT;
    connect( client, SIGNAL( senemylist( bool ) ), this, SLOT( setEnemyList( bool ) ) );
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

bool KonnectionHandling::haveEnemy()
{
    return enemy;
}

bool KonnectionHandling::gotEnemyList()
{
    return enemylist;
}

void KonnectionHandling::setEnemyList( bool set )
{
    senemylist = set;
}

bool KonnectionHandling::sendEnemyList()
{
    return senemylist;
}

void KonnectionHandling::serverGotNewClient()
{
    enemy = true;
    KMessageBox::information( 0L, i18n( "We got a player. Let's start..." ) );
    emit statusBarMessage( i18n( "Please place your ships" ) );
}

void KonnectionHandling::serverWroteToClient()
{
}

void KonnectionHandling::serverLostClient()
{
    kdDebug() << "ENDCLIENT!" << endl;
}

bool KonnectionHandling::writeable()
{
    switch( getType() )
    {
	case KonnectionHandling::CLIENT:
	    return internalClient->write();
	    break;
	    
	case KonnectionHandling::SERVER:
	    return internalServer->write();
	    break;
	
	default: 
	    break;
    }
    return false;
}

void KonnectionHandling::gotNewMessage( KMessage *msg )
{
    switch( getType() )
    {
	case KonnectionHandling::CLIENT:
	    switch( msg->getType() )
	    {
		case KMessage::GREET:
		    emit enemyNickname( msg->getField( "nickname" ) );
		    break;
		    
		case KMessage::SHIPLIST:
		    enemylist = true;
		    emit gotEnemyShipList( msg->getField( "fieldx1s1" ), msg->getField( "fieldy1s1" ), msg->getField( "fieldx2s1" ), msg->getField( "fieldy2s1" ), msg->getField( "fieldx1s2" ), msg->getField( "fieldy2s2" ), msg->getField( "fieldx2s2" ), msg->getField( "fieldy2s2" ), msg->getField( "fieldx1s3" ), msg->getField( "fieldy1s3" ), msg->getField( "fieldx2s3" ), msg->getField( "fieldy2s3" ), msg->getField( "fieldx1s4" ), msg->getField( "fieldy2s4" ), msg->getField( "fieldx2s4" ), msg->getField( "fieldy2s4" ) );
		    emit setPlaceable();
		    emit statusBarMessage( i18n( "Please place your ships" ) );
		    break;
			
	        case KMessage::ANSWER_SHOOT:
		    emit ownFieldDataChanged( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt(), msg->getField( "fieldstate" ).toInt() );
		    emit statusBarMessage( i18n( "Enemy has shot | Shoot now" ) );
		    break;
		    
		case KMessage::CHAT:
		    emit gotChatMessage( msg->getField( "nickname" ), msg->getField( "chat" ) );
		    break;
    	    }
	    if( msg->getField( "enemy" ) == QString( "ready" ) )
		internalClient->allowWrite();
	    break;
	    
	case KonnectionHandling::SERVER:
	    switch( msg->getType() )
	    {
		case KMessage::GREET:
		    emit enemyNickname( msg->getField( "nickname" ) );
		    emit giveEnemyName();
		    break;
		    
		case KMessage::SHIPLIST:
		    enemylist = true;
		    emit gotEnemyShipList( msg->getField( "fieldx1s1" ), msg->getField( "fieldy1s1" ), msg->getField( "fieldx2s1" ), msg->getField( "fieldy2s1" ), msg->getField( "fieldx1s2" ), msg->getField( "fieldy2s2" ), msg->getField( "fieldx2s2" ), msg->getField( "fieldy2s2" ), msg->getField( "fieldx1s3" ), msg->getField( "fieldy1s3" ), msg->getField( "fieldx2s3" ), msg->getField( "fieldy2s3" ), msg->getField( "fieldx1s4" ), msg->getField( "fieldy2s4" ), msg->getField( "fieldx2s4" ), msg->getField( "fieldy2s4" ) );
		    emit statusBarMessage( i18n( "You can shoot now" ) );
		    break;
		    
		case KMessage::ANSWER_SHOOT:
		    emit ownFieldDataChanged( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt(), msg->getField( "fieldstate" ).toInt() );
		    emit statusBarMessage( i18n( "Enemy has shot | Shoot now" ) );
		    break;
		    
		case KMessage::CHAT:
		    emit gotChatMessage( msg->getField( "nickname" ), msg->getField( "chat" ) );
		    break;
	    }
	    if( msg->getField( "enemy" ) == QString( "ready" ) )
		internalServer->allowWrite();
	    break;
    }
}
    
void KonnectionHandling::clientLostServer()
{
    KMessageBox::error( 0L, i18n( "Connection to server lost. Aborting the game!" ) );
    emit abortGame();
}

void KonnectionHandling::clientSocketError( int error )
{
    switch( error )
    {
	case QSocket::HostLookup:
	    KMessageBox::error( 0L, i18n( "Couldn't lookup host!" ) );
	    emit changeConnectText();
	    break;
	
	case QSocket::Connecting:
/*	    if( !timer->isActive() )
	    {
		timer = new QTimer( myObject );
	        connect( timer, SIGNAL( timeout() ), myObject, SLOT( timerDone() ) );
		timer->start( 10000, true );
	    }
*/	    break;
	
	default:
	    KMessageBox::error( 0L, i18n( "Unknown Error; No: %1" ).arg( error ) ) ;
	    break;
    }
}
