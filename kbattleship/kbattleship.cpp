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
    setMinimumSize( 680, 480 );
}

KBattleshipApp::~KBattleshipApp()
{
}

void KBattleshipApp::init()
{
    haveCS = false;
    place = false;
    config = kapp->config();
    initStatusBar();
    initActions();
    initView();
    initChat();
    initSound();
    initShipPlacing();
    readOptions();
}

void KBattleshipApp::initActions()
{
    gameServerConnect = new KAction( i18n( "&Connect to server" ), "connect_no", Key_F2, this, SLOT( slotServerConnect() ), actionCollection(), "serverconnect" );
    gameNewServer = new KAction( i18n( "&Start server" ), "network", Key_F3, this, SLOT( slotNewServer() ), actionCollection(), "newserver" );
    gameQuit = KStdAction::quit( this, SLOT( slotGameQuit() ), actionCollection() );
    viewToolBar = KStdAction::showToolbar( this, SLOT( slotViewToolBar() ), actionCollection() );
    viewStatusBar = KStdAction::showStatusbar( this, SLOT( slotViewStatusBar() ), actionCollection() );

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

void KBattleshipApp::initChat()
{
    connect( chat, SIGNAL( sendMessage( QString ) ), this, SLOT( sendChatMessage( QString ) ) );
}

void KBattleshipApp::initShipPlacing()
{
    connect( ownshiplist, SIGNAL( ownFieldDataChanged( int, int, int ) ), this, SLOT( changeOwnFieldData( int, int, int ) ) );
}

void KBattleshipApp::initStatusBar()
{
    statusBar()->insertItem( i18n( "Ready" ), ID_STATUS_MSG );
}

void KBattleshipApp::initView()
{
    splitV = new QSplitter( QSplitter::Vertical, this );
    splitH = new QSplitter( QSplitter::Horizontal, splitV );
    view = new KBattleshipView( splitH );

    chat = new KChatWidget( splitV );
    stat = new KStatDialog( splitH );
    
    chat->acceptMsg( false );

    ownshiplist = new KShipList();
    enemyshiplist = new KShipList();

    view->startDrawing();

    setCentralWidget( splitV );

    connect( ownshiplist, SIGNAL( lastShipAdded() ), this, SLOT( sendShipList() ) );    
    connect( view, SIGNAL( enemyFieldClicked( int, int ) ), this, SLOT( enemyClick( int, int ) ) );
    connect( view, SIGNAL( ownFieldClicked( int, int, int ) ), this, SLOT( placeShip( int, int, int ) ) );
    connect( view, SIGNAL( requestedOwnFieldShipListJob( int, int, QPainter *, bool, bool ) ), this, SLOT( requestedOwnFieldShipListJob( int, int, QPainter *, bool, bool ) ) );
    connect( view, SIGNAL( requestedEnemyFieldShipListJob( int, int, QPainter * ) ), this, SLOT( requestedEnemyFieldShipListJob( int, int, QPainter * ) ) );
    
    setCaption( i18n( "KBattleship (beta2)" ), false );
}

void KBattleshipApp::enemyClick( int fieldx, int fieldy )
{
    if( haveCS )
    {
	if( connection->writeable() && connection->gotEnemyList() && connection->sendEnemyList() )
	{
    	    if( view->getEnemyFieldState( fieldx, fieldy ) == KBattleField::FREE )
	    {
		int showstate;
		int enemystate = enemyshiplist->getXYShipType( fieldx, fieldy );
		stat->setShot();
		if( enemystate == 99 )
		{
		    stat->setWater();
		    showstate = KBattleField::WATER;
		}
		else
		{
		    stat->setHit();
		    showstate = KBattleField::HIT;
		}
	    
		changeEnemyFieldData( fieldx, fieldy, showstate );
		sendMessage( fieldx, fieldy, showstate );
		slotStatusMsg( i18n( "Waiting for enemy to shoot.." ) );
	    }
	}
    }
}

void KBattleshipApp::placeShip( int fieldx, int fieldy, int button )
{
    if( haveCS && connection->haveEnemy() )
	if( connection->getType() == KonnectionHandling::CLIENT )
	{
	    if( place )
	    {
		if( ownshiplist->canAddShips() )
		    ownshiplist->addNewShip( button, fieldx, fieldy );
	    }
	}
	else
	{
	    if( ownshiplist->canAddShips() )
		ownshiplist->addNewShip( button, fieldx, fieldy );
	}
}

void KBattleshipApp::sendShipList()
{
    KMessage *msg = new KMessage( KMessage::SHIPLIST );
    QString fieldX1S1Data, fieldX2S1Data, fieldX1S2Data, fieldX2S2Data;
    QString fieldX1S3Data, fieldX2S3Data, fieldX1S4Data, fieldX2S4Data;
    QString fieldY1S1Data, fieldY2S1Data, fieldY1S2Data, fieldY2S2Data;
    QString fieldY1S3Data, fieldY2S3Data, fieldY1S4Data, fieldY2S4Data;
    fieldX1S1Data.setNum( ownshiplist->returnX1ship( 1 ) );
    fieldX2S1Data.setNum( ownshiplist->returnX2ship( 1 ) );
    fieldX1S2Data.setNum( ownshiplist->returnX1ship( 2 ) );
    fieldX2S2Data.setNum( ownshiplist->returnX2ship( 2 ) );
    fieldX1S3Data.setNum( ownshiplist->returnX1ship( 3 ) );
    fieldX2S3Data.setNum( ownshiplist->returnX2ship( 3 ) );
    fieldX1S4Data.setNum( ownshiplist->returnX1ship( 4 ) );
    fieldX2S4Data.setNum( ownshiplist->returnX2ship( 4 ) );
    fieldY1S1Data.setNum( ownshiplist->returnY1ship( 1 ) );
    fieldY2S1Data.setNum( ownshiplist->returnY2ship( 1 ) );
    fieldY1S2Data.setNum( ownshiplist->returnY1ship( 2 ) );
    fieldY2S2Data.setNum( ownshiplist->returnY2ship( 2 ) );
    fieldY1S3Data.setNum( ownshiplist->returnY1ship( 3 ) );
    fieldY2S3Data.setNum( ownshiplist->returnY2ship( 3 ) );
    fieldY1S4Data.setNum( ownshiplist->returnY1ship( 4 ) );
    fieldY2S4Data.setNum( ownshiplist->returnY2ship( 4 ) );

    msg->addField( QString( "fieldx1s1" ), fieldX1S1Data );
    msg->addField( QString( "fieldx2s1" ), fieldX2S1Data );
    msg->addField( QString( "fieldx1s2" ), fieldX1S2Data );
    msg->addField( QString( "fieldx2s2" ), fieldX2S2Data );
    msg->addField( QString( "fieldx1s3" ), fieldX1S3Data );
    msg->addField( QString( "fieldx2s3" ), fieldX2S3Data );
    msg->addField( QString( "fieldx1s4" ), fieldX1S4Data );
    msg->addField( QString( "fieldx2s4" ), fieldX2S4Data );
    msg->addField( QString( "fieldy1s1" ), fieldY1S1Data );
    msg->addField( QString( "fieldy2s1" ), fieldY2S1Data );
    msg->addField( QString( "fieldy1s2" ), fieldY1S2Data );
    msg->addField( QString( "fieldy2s2" ), fieldY2S2Data );
    msg->addField( QString( "fieldy1s3" ), fieldY1S3Data );
    msg->addField( QString( "fieldy2s3" ), fieldY2S3Data );
    msg->addField( QString( "fieldy1s4" ), fieldY1S4Data );
    msg->addField( QString( "fieldy2s4" ), fieldY2S4Data );

    msg->addField( QString( "enemy" ), QString( "ready" ) );

    sendMessage( msg );

    if( connection->getType() == KonnectionHandling::SERVER )
	slotStatusMsg( i18n( "Waiting for other player to place the ships..." ) );

    if( connection->getType() == KonnectionHandling::CLIENT )
	slotStatusMsg( i18n( "Waiting for other player to start the match..." ) );

}

void KBattleshipApp::sendMessage( int fieldx, int fieldy, int state )
{
    if( haveCS )
    {
	KMessage *msg = new KMessage( KMessage::ANSWER_SHOOT );
	QString field1Data, field2Data, field3Data;
	field1Data.setNum( fieldx );
	field2Data.setNum( fieldy );
	field3Data.setNum( state );
	msg->addField( QString( "fieldx" ), field1Data );
	msg->addField( QString( "fieldy" ), field2Data );
	msg->addField( QString( "fieldstate" ), field3Data );
	msg->addField( QString( "enemy" ), QString( "ready" ) );
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

void KBattleshipApp::sendChatMessage( QString text )
{
    if( haveCS )
    {
	bool currentstate = connection->writeable();
	bool serverallow = false, clientallow = false, isserver = false, isclient = false;
	if( !currentstate )
	{	
	    switch( connection->getType() )
	    {
		case KonnectionHandling::SERVER:
		    serverallow = true;
		    isserver = true;
		    break;
		
		case KonnectionHandling::CLIENT:
		    clientallow = true;
		    isclient = true;
		    break;
	    }
	}
	else
	{
	    switch( connection->getType() )
	    {
		case KonnectionHandling::SERVER:
		    isserver = true;
		    break;
		
		case KonnectionHandling::CLIENT:
		    isclient = true;
		    break;
	    }
	}
	if( serverallow )
	    kbserver->allowWrite();
	if( clientallow )
	    kbclient->allowWrite();
	KMessage *msg = new KMessage( KMessage::CHAT );
    	msg->chatMessage( currentNickname, text );
	if( isserver )
	    kbserver->sendMessage( msg );
	if( isclient )
	    kbclient->sendMessage( msg );
	delete msg;
	if( serverallow )
	    kbserver->forbidWrite();
	if( clientallow )
	    kbclient->forbidWrite();
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
    slotStatusMsg( i18n( "Ready" ) );
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
	// TODO: use KDialogBase
        slotStatusMsg( i18n( "Loading Connect-Server dialog..." ) );

	client = new KClientDialog();
	haveCS = true;
        connect( client, SIGNAL( connectServer() ), this, SLOT( connectToBattleshipServer() ) );
	connect( client, SIGNAL( cancelConnect() ), this, SLOT( stoppedConnectDialog() ) );
        client->show();

        slotStatusMsg( i18n( "Ready" ) );
    }
    else
    {
	switch( connection->getType() )
	{
	    case KonnectionHandling::SERVER:
		KMessageBox::error( this, i18n( "You can't connect to a server while you running your own!" ) );
		break;
		
	    case KonnectionHandling::CLIENT:
		resetClient();
		break;
	}    
    }
}

void KBattleshipApp::resetClient()
{
    gameServerConnect->setText( "&Connect to server" );
    view->clearField();
    delete connection;
    delete kbclient;
    connection = 0;
    stoppedConnectDialog();
    delete ownshiplist;
    delete enemyshiplist;
    ownshiplist = new KShipList();
    enemyshiplist = new KShipList();
}
void KBattleshipApp::resetServer()
{
    gameNewServer->setText( "&Start server" );
    view->clearField();
    haveCS = false;
    delete connection;
    delete kbserver;
    connection = 0;
    stoppedServerDialog();
    delete ownshiplist;
    delete enemyshiplist;
    ownshiplist = new KShipList();
    enemyshiplist = new KShipList();
}

void KBattleshipApp::slotNewServer()
{
    if( !haveCS )
    {
	// TODO: use KDialogBase
        slotStatusMsg( i18n( "Loading Start-Server dialog..." ) );
    
	server = new KServerDialog();
	haveCS = true;
	connect( server, SIGNAL( startServer() ), this, SLOT( startBattleshipServer() ) );
	connect( server, SIGNAL( cancelServer() ), this, SLOT( stoppedServerDialog() ) );
        server->show();
            
	slotStatusMsg( i18n( "Ready" ) );
    }
    else
    {
	switch( connection->getType() )
	{
	    case KonnectionHandling::SERVER:
		resetServer();
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
    currentNickname = server->getNickname();
    chat->setNickname( currentNickname );
    delete server;
    connect( kbserver, SIGNAL( serverFailure() ), this, SLOT( changeStartText() ) );
    kbserver->start();
    chat->acceptMsg( true );
    connection = new KonnectionHandling( this, kbserver );    
    connect( connection, SIGNAL( statusBarMessage( const QString & ) ), this, SLOT( slotStatusMsg( const QString & ) ) );
    connect( connection, SIGNAL( ownFieldDataChanged( int, int, int ) ), this, SLOT( changeOwnFieldData( int, int, int ) ) );
    connect( connection, SIGNAL( enemyFieldDataChanged( int, int, int ) ), this, SLOT( changeEnemyFieldData( int, int, int ) ) );
    connect( connection, SIGNAL( sendMessage( KMessage * ) ), this, SLOT( sendMessage( KMessage * ) ) );
    connect( connection, SIGNAL( gotChatMessage( QString, QString ) ), chat, SLOT( receivedMessage( QString, QString ) ) );
    connect( connection, SIGNAL( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ), this, SLOT( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );
    
    slotStatusMsg( i18n( "Waiting for a player..." ) );
}

void KBattleshipApp::changeOwnFieldData( int fieldx, int fieldy, int type )
{
    view->changeOwnFieldData( fieldx, fieldy, type );
}

void KBattleshipApp::requestedOwnFieldShipListJob( int fieldx, int fieldy, QPainter *painter, bool hit, bool death )
{
    view->giveOwnFieldShipListType( painter, ownshiplist->getXYShipType( fieldx, fieldy ), hit, death );
}

void KBattleshipApp::requestedEnemyFieldShipListJob( int fieldx, int fieldy, QPainter *painter )
{
    view->giveEnemyFieldShipListType( painter, enemyshiplist->getXYShipType( fieldx, fieldy ) );
}

void KBattleshipApp::changeEnemyFieldData( int fieldx, int fieldy, int type )
{
    switch( connection->getType() )
    {
	case KonnectionHandling::SERVER:
	    switch( type )
	    {
		case KBattleField::WATER:
		    sound->playSound( KBattleshipSound::PLAYER_SHOOT_WATER );
		    break;
		    
		case KBattleField::HIT:
		    sound->playSound( KBattleshipSound::PLAYER1_SHOOT_HIT );
		    break;
		
		case KBattleField::DEATH:
		    sound->playSound( KBattleshipSound::SHIP_SINK );
		    break;
	    }
	    break;
	    
	case KonnectionHandling::CLIENT:
	    switch( type )
	    {
		case KBattleField::WATER:
		    sound->playSound( KBattleshipSound::PLAYER_SHOOT_WATER );
		    break;
		    
		case KBattleField::HIT:
		    sound->playSound( KBattleshipSound::PLAYER2_SHOOT_HIT );
		    break;
		
		case KBattleField::DEATH:
		    sound->playSound( KBattleshipSound::SHIP_SINK );
		    break;
	    }
	    break;

    }
    view->changeEnemyFieldData( fieldx, fieldy, type );
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
	currentNickname = client->getNickname();
	chat->setNickname( currentNickname );
	delete client;
        gameServerConnect->setText( "Dis&connect from server" );
	chat->acceptMsg( true );
        connection = new KonnectionHandling( this, kbclient );
	connect( connection, SIGNAL( statusBarMessage( const QString & ) ), this, SLOT( slotStatusMsg( const QString & ) ) );
	connect( connection, SIGNAL( ownFieldDataChanged( int, int, int ) ), this, SLOT( changeOwnFieldData( int, int, int ) ) );
	connect( connection, SIGNAL( setPlaceable() ), this, SLOT( setPlaceable() ) );
	connect( connection, SIGNAL( abortGame() ), this, SLOT( resetClient() ) );
	connect( connection, SIGNAL( enemyFieldDataChanged( int, int, int ) ), this, SLOT( changeEnemyFieldData( int, int, int ) ) );
        connect( connection, SIGNAL( changeConnectText() ), this, SLOT( changeConnectText() ) );
        connect( connection, SIGNAL( sendMessage( KMessage * ) ), this, SLOT( sendMessage( KMessage * ) ) );
	connect( connection, SIGNAL( gotChatMessage( QString, QString ) ), chat, SLOT( receivedMessage( QString, QString ) ) );
	connect( connection, SIGNAL( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ), this, SLOT( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );
	
	slotStatusMsg( i18n( "Waiting for other player to place the ships..." ) );
    }
    else
    {
	KMessageBox::error( this, i18n( "You forgot to enter a host!" ) );
	haveCS = false;
    }
}

void KBattleshipApp::setPlaceable()
{
    place = true;
}

void KBattleshipApp::gotEnemyShipList( QString fieldX1S1, QString fieldY1S1, QString fieldX2S1, QString fieldY2S1, QString fieldX1S2, QString fieldY1S2, QString fieldX2S2, QString fieldY2S2, QString fieldX1S3, QString fieldY1S3, QString fieldX2S3, QString fieldY2S3, QString fieldX1S4, QString fieldY1S4, QString fieldX2S4, QString fieldY2S4 )
{
    enemyshiplist->addShip1( fieldX1S1.toInt(), fieldX2S1.toInt(), fieldY1S1.toInt(), fieldY2S1.toInt() );
    enemyshiplist->addShip2( fieldX1S2.toInt(), fieldX2S2.toInt(), fieldY1S2.toInt(), fieldY2S2.toInt() );
    enemyshiplist->addShip3( fieldX1S3.toInt(), fieldX2S3.toInt(), fieldY1S3.toInt(), fieldY2S3.toInt() );
    enemyshiplist->addShip4( fieldX1S4.toInt(), fieldX2S4.toInt(), fieldY1S4.toInt(), fieldY2S4.toInt() );
}

void KBattleshipApp::slotViewToolBar()
{
    slotStatusMsg( i18n( "Toggling the toolbar..." ) );
    if( !viewToolBar->isChecked() )
        toolBar()->hide();
    else
        toolBar()->show();

    slotStatusMsg( i18n( "Ready" ) );
}

void KBattleshipApp::slotViewStatusBar()
{
    slotStatusMsg( i18n( "Toggling the statusbar..." ) );
    if( !viewStatusBar->isChecked() )
        statusBar()->hide();
    else
        statusBar()->show();

    slotStatusMsg( i18n( "Ready" ) );
}

void KBattleshipApp::slotStatusMsg( const QString &text )
{
    statusBar()->clear();
    statusBar()->changeItem( text, ID_STATUS_MSG );
}
