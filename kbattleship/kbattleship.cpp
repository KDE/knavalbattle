/***************************************************************************
                                  kbattleship.cpp
                                  -----------------
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

#include "kbattleship.moc"

using namespace Arts;
using namespace std;

#define ID_STATUS_MSG 1

KBattleshipApp::KBattleshipApp( QWidget *, const char *name ) : KMainWindow( 0, name )
{
    haveCS = false;
    config = kapp->config();
    initStatusBar();
    initActions();
    initView();
    initSound();
    readOptions();
}

KBattleshipApp::~KBattleshipApp()
{
}

void KBattleshipApp::initActions()
{
    gameServerConnect = new KAction( i18n( "&Connect to server" ), "connect_no", Key_F2, this,
                               SLOT(slotServerConnect()), actionCollection(), "serverconnect" );
    gameNewServer = new KAction( i18n( "&Start server" ), "network", Key_F3, this,
                               SLOT( slotNewServer() ), actionCollection(), "newserver" );
    gameQuit = KStdAction::quit(this, SLOT(slotGameQuit()), actionCollection());
    viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
    viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());

    gameServerConnect->setStatusText( i18n( "Connects to a server for a new game" ) );
    gameNewServer->setStatusText( i18n( "Opens a server game" ) );
    gameQuit->setStatusText( i18n( "Quits the application" ) );
    gameQuit->setStatusText( i18n( "Quits the application" ) );
    viewToolBar->setStatusText( i18n( "Enables/disables the toolbar" ) );
    viewStatusBar->setStatusText( i18n( "Enables/disables the statusbar" ) );

    createGUI();
}

void KBattleshipApp::initSound()
{
    new Dispatcher;
    sound = new KBattleshipSound();
}

void KBattleshipApp::initStatusBar()
{
    statusBar()->insertItem( i18n( "Ready." ), ID_STATUS_MSG );
}

void KBattleshipApp::initView()
{
    view = new KBattleshipView( this );
    shiplist = new KShipList();    
    
    setCentralWidget( view );
    view->startDrawing();
    
    connect( view, SIGNAL( enemyFieldClicked( int, int ) ), this, SLOT( sendMessage( int, int ) ) );
    connect( view, SIGNAL( ownFieldClicked( int, int, int ) ), this, SLOT( placeShip( int, int, int ) ) );
    
    setCaption( i18n( "KBattleship (pre-alpha)" ), false );
}

void KBattleshipApp::placeShip( int fieldx, int fieldy, int button )
{
    if( haveCS )
    {
        if( shiplist->canAddShips() )
        {
	    switch( button )
	    {
		case LeftButton:
		    if( fieldx + shiplist->shipCount() <= 8 )
		    {
			shiplist->addNewShip( fieldx, fieldy, shiplist->shipCount() );
			switch( shiplist->shipCount() )
			{
			    case 3:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP4P0 );
				view->changeOwnFieldData( fieldx + 1, fieldy, KShipType::SHIP4P1 );
				view->changeOwnFieldData( fieldx + 2, fieldy, KShipType::SHIP4P2 );
				view->changeOwnFieldData( fieldx + 3, fieldy, KShipType::SHIP4P3 );
				break;
			
			    case 2:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP3P0 );
				view->changeOwnFieldData( fieldx + 1, fieldy, KShipType::SHIP3P1 );
				view->changeOwnFieldData( fieldx + 2, fieldy, KShipType::SHIP3P2 );
				break;

			    case 1:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP2P0 );
				view->changeOwnFieldData( fieldx + 1, fieldy, KShipType::SHIP2P1 );
				break;

			    case 0:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP1P0 );
				break;
	    		}
		    }
		    break;
			
		case RightButton:
		    if( fieldy + shiplist->shipCount() <= 8 )
		    {
			shiplist->addNewShip( fieldx, fieldy, shiplist->shipCount() );
			switch( shiplist->shipCount() )
			{
			    case 3:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP4P0 );
				view->changeOwnFieldData( fieldx, fieldy + 1, KShipType::SHIP4P1 );
				view->changeOwnFieldData( fieldx, fieldy + 2, KShipType::SHIP4P2 );
				view->changeOwnFieldData( fieldx, fieldy + 3, KShipType::SHIP4P3 );
				break;
			
			    case 2:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP3P0 );
				view->changeOwnFieldData( fieldx, fieldy + 1, KShipType::SHIP3P1 );
				view->changeOwnFieldData( fieldx, fieldy + 2, KShipType::SHIP3P2 );
				break;

			    case 1:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP2P0 );
				view->changeOwnFieldData( fieldx, fieldy + 1, KShipType::SHIP2P1 );
				break;

			    case 0:
				view->changeOwnFieldData( fieldx, fieldy, KShipType::SHIP1P0 );
				break;
	    		}
		    }
		    break;
	    }
        }
    }
}


void KBattleshipApp::sendMessage( int fieldx, int fieldy )
{
    if( haveCS )
    {
	KMessage *msg = new KMessage( KMessage::ENEMY_SHOOT );
	QString field1Data; 
	QString field2Data;
	field1Data.setNum( fieldx );
	field2Data.setNum( fieldy );
	msg->addField( QString( "fieldx" ), field1Data );
	msg->addField( QString( "fieldy" ), field2Data );

	switch( connection->getType() )
	{
	    case KonnectionHandling::SERVER:
		kbserver->sendMessage( msg );
		delete msg;
		break;
		
	    case KonnectionHandling::CLIENT:
		kbclient->sendMessage( msg );
		delete msg;
		break;	
	}
    }
}

void KBattleshipApp::sendMessage( KMessage *msg )
{
    if( haveCS )
    {
	switch( connection->getType() )
	{
	    case KonnectionHandling::SERVER:
		kbserver->sendMessage( msg );
		delete msg;
		break;
		
	    case KonnectionHandling::CLIENT:
		kbclient->sendMessage( msg );
		delete msg;
		break;	
	}
    }
}

void KBattleshipApp::saveOptions()
{
    config->setGroup( "General" );
    config->writeEntry( "ShowToolbar", viewToolBar->isChecked() );
    config->writeEntry( "ShowStatusbar", viewStatusBar->isChecked() );
    config->writeEntry( "ToolBarPos", (int) toolBar()->barPos() );
}

void KBattleshipApp::readOptions()
{
    config->setGroup( "General" );

    bool bViewToolbar = config->readBoolEntry( "ShowToolbar", true );
    viewToolBar->setChecked( bViewToolbar );
    slotViewToolBar();

    bool bViewStatusbar = config->readBoolEntry( "ShowStatusbar", true );
    viewStatusBar->setChecked( bViewStatusbar );
    slotViewStatusBar();

    KToolBar::BarPosition toolBarPos;
    toolBarPos = ( KToolBar::BarPosition ) config->readNumEntry( "ToolBarPos", KToolBar::Top );
    toolBar()->setBarPos( toolBarPos );
}

void KBattleshipApp::slotGameQuit()
{
    slotStatusMsg( i18n( "Exiting..." ) );

    saveOptions();

    slotStatusMsg( i18n( "Ready." ) );
    
    this->hide();
    
    exit( 0 );
}

void KBattleshipApp::stoppedServerDialog()
{
    haveCS = false;
}

void KBattleshipApp::stoppedConnectDialog()
{
    haveCS = false;
}

void KBattleshipApp::slotServerConnect()
{
    if( !haveCS )
    {
        slotStatusMsg( i18n( "Loading Connect-Server dialog..." ) );

	// TODO: use KDialogBase

        client = new KClientDialog();
	haveCS = true;
        connect( client, SIGNAL( connectServer() ), this, SLOT( connectToBattleshipServer() ) );
	connect( client, SIGNAL( cancelConnect() ), this, SLOT( stoppedConnectDialog() ) );
        client->show();

        slotStatusMsg( i18n( "Ready." ) );
    }
    else
    {
	switch( connection->getType() )
	{
	    case KonnectionHandling::SERVER:
		KMessageBox::error( this, i18n( "You can't connect to a server while you running your own!" ) );
		break;
		
	    case KonnectionHandling::CLIENT:
	        gameServerConnect->setText( "&Connect to server" );
		view->clearField();
		delete connection;
		delete kbclient;
		connection = 0;
		break;
	}
	    
    }
}

void KBattleshipApp::slotNewServer()
{
    if( !haveCS )
    {
        slotStatusMsg( i18n( "Loading Start-Server dialog..." ) );
    
	// TODO: use KDialogBase
    
        server = new KServerDialog();
	haveCS = true;
	connect( server, SIGNAL( startServer() ), this, SLOT( startBattleshipServer() ) );
	connect( server, SIGNAL( cancelServer() ), this, SLOT( stoppedServerDialog() ) );
        server->show();
            
	slotStatusMsg( i18n( "Ready." ) );
    }
    else
    {
	switch( connection->getType() )
	{
	    case KonnectionHandling::SERVER:
	        gameNewServer->setText( "&Start server" );
		view->clearField();
		haveCS = false;
		delete connection;
		delete kbserver;
		connection = 0;
		break;
	
	    case KonnectionHandling::CLIENT:
		KMessageBox::error( this, i18n( "You can't start a server while you are connected!" ) );
		break;
	}
    }
}

void KBattleshipApp::startBattleshipServer()
{
    gameNewServer->setText( "&Stop server" );
    kbserver = new KBattleshipServer( ( server->getPort() ).toInt() );
    delete server;
    connect( kbserver, SIGNAL( serverFailure() ), this, SLOT( changeStartText() ) );
    kbserver->start();

    connection = new KonnectionHandling( this, kbserver );    

    connect( connection, SIGNAL( ownFieldDataChanged( int, int, int ) ), this, SLOT( changeOwnFieldData( int, int, int ) ) );
    connect( connection, SIGNAL( enemyFieldDataChanged( int, int, int ) ), this, SLOT( changeEnemyFieldData( int, int, int ) ) );
    connect( connection, SIGNAL( requestBattleFieldState( int, int ) ), this, SLOT( requestedOwnBattleFieldState( int, int ) ) );
    connect( connection, SIGNAL( sendMessage( KMessage * ) ), this, SLOT( sendMessage( KMessage * ) ) );
    connect( this, SIGNAL( battleFieldState( int, int, int ) ), connection, SLOT( gotBattleFieldState( int, int, int ) ) );
}

void KBattleshipApp::changeOwnFieldData( int fieldx, int fieldy, int type )
{
    view->changeOwnFieldData( fieldx, fieldy, 1 ); //type );
}

void KBattleshipApp::requestedOwnBattleFieldState( int fieldx, int fieldy )
{
    int state = view->getOwnFieldState( fieldx, fieldy );
    emit battleFieldState( fieldx, fieldy, state );
}

void KBattleshipApp::requestedEnemyBattleFieldState( int fieldx, int fieldy )
{
    int state = view->getEnemyFieldState( fieldx, fieldy );
    emit battleFieldState( fieldx, fieldy, state );
}

void KBattleshipApp::changeEnemyFieldData( int fieldx, int fieldy, int type )
{
    kdDebug() << "Changing enemyfielddata! ( " << fieldx << " | " << fieldy << " )" << endl;
//    switch( type )
//    {
//	case KBattleshipSound::PLAYER1_SHOOT_HIT:
//    }
    sound->playSound( KBattleshipSound::PLAYER_SHOOT_WATER );
    view->changeEnemyFieldData( fieldx, fieldy, 1 ); //type );
}

void KBattleshipApp::changeConnectText()
{
    gameServerConnect->setText( "&Connect to server" );
    haveCS = false;
}

void KBattleshipApp::changeStartText()
{
    gameNewServer->setText( "&Start server" );
    haveCS = false;
}

void KBattleshipApp::connectToBattleshipServer()
{
    if( client->getHost() != "" )
    {
	kbclient = new KBattleshipClient( client->getHost(), ( client->getPort() ).toInt() );
        delete client;
        gameServerConnect->setText( "Dis&connect from server" );
        connection = new KonnectionHandling( this, kbclient );
	connect( connection, SIGNAL( ownFieldDataChanged( int, int, int ) ), this, SLOT( changeOwnFieldData( int, int, int ) ) );
	connect( connection, SIGNAL( enemyFieldDataChanged( int, int, int ) ), this, SLOT( changeEnemyFieldData( int, int, int ) ) );
	connect( connection, SIGNAL( requestBattleFieldState( int, int ) ), this, SLOT( requestedEnemyBattleFieldState( int, int ) ) );
        connect( connection, SIGNAL( changeConnectText() ), this, SLOT( changeConnectText() ) );
        connect( connection, SIGNAL( sendMessage( KMessage * ) ), this, SLOT( sendMessage( KMessage * ) ) );
	connect( this, SIGNAL( battleFieldState( int, int, int ) ), connection, SLOT( gotBattleFieldState( int, int, int ) ) );
    }
    else
    {
	KMessageBox::error( this, i18n( "You forgot to enter a host!" ) );
	haveCS = false;
    }
}

void KBattleshipApp::slotViewToolBar()
{
    slotStatusMsg( i18n( "Toggling the toolbar..." ) );
    if( !viewToolBar->isChecked() )
    {
        toolBar()->hide();
    }
    else
    {
        toolBar()->show();
    }

    slotStatusMsg( i18n( "Ready." ) );
}

void KBattleshipApp::slotViewStatusBar()
{
    slotStatusMsg( i18n( "Toggling the statusbar..." ) );
    if( !viewStatusBar->isChecked() )
    {
        statusBar()->hide();
    }
    else
    {
        statusBar()->show();
    }

    slotStatusMsg( i18n( "Ready." ) );
}


void KBattleshipApp::slotStatusMsg( const QString &text )
{
    statusBar()->clear();
    statusBar()->changeItem( text, ID_STATUS_MSG );
}

