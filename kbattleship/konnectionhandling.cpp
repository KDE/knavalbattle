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
    KMessage *msg = new KMessage( KMessage::ANSWER_SHOOT );
    QString qstate;
    QString qfieldx;
    QString qfieldy;
    qstate.setNum( state );    
    qfieldx.setNum( fieldx );
    qfieldy.setNum( fieldy );
    msg->addField( QString( "fieldx" ), qfieldx );
    msg->addField( QString( "fieldy" ), qfieldy );
    msg->addField( QString( "fieldstate" ), qstate );
    emit sendMessage( msg );
    if( state == KBattleField::SHIP )
	emit ownFieldDataChanged( fieldx, fieldy, KBattleField::HIT );
}

void KonnectionHandling::gotNewMessage( KMessage *msg )
{
    switch( getType() )
    {
	case KonnectionHandling::CLIENT:
	    switch( msg->getType() )
	    {
		case KMessage::ENEMY_SHOOT:
		    emit requestBattleFieldState( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt() );
    		    break;
			
	        case KMessage::ANSWER_SHOOT:
		    int state;
		    if( msg->getField( "fieldstate" ).toInt() == KBattleField::SHIP )
			state = KBattleField::HIT;
		    else
			state = KBattleField::WATER;
					    
		    emit enemyFieldDataChanged( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt(), state );
		    break;
		    
		case KMessage::CHAT:
		    emit gotChatMessage( &(msg->getField( "nickname" )), &(msg->getField( "chat" )) );
		    break;
    	    }
	    break;
	    
	case KonnectionHandling::SERVER:
	    switch( msg->getType() )
	    {
		case KMessage::ENEMY_SHOOT:
		    emit requestBattleFieldState( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt() );
		    break;
		    
		case KMessage::ANSWER_SHOOT:
		    int state;
		    if( msg->getField( "fieldstate" ).toInt() == KBattleField::SHIP )
			state = KBattleField::HIT;
		    else
			state = KBattleField::WATER;
					    
		    emit enemyFieldDataChanged( msg->getField( "fieldx" ).toInt(), msg->getField( "fieldy" ).toInt(), state );
		    break;
		    
		case KMessage::CHAT:
		    emit gotChatMessage( &(msg->getField( "nickname" )), &(msg->getField( "chat" )) );
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
