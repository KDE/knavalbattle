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
#define ID_PLAYER_OWN 2
#define ID_PLAYER_ENEMY 3

KBattleshipApp::KBattleshipApp( QWidget *, const char *name ) : KMainWindow( 0, name )
{
    setMinimumSize( 680, 480 );
}

KBattleshipApp::~KBattleshipApp()
{
    saveOptions();
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
    configSound = new KToggleAction( i18n( "&Play sounds" ), 0, this, SLOT( slotConfigSound() ), actionCollection(), "configsound" );
    gameQuit = KStdAction::quit( this, SLOT( slotGameQuit() ), actionCollection() );
    viewToolBar = KStdAction::showToolbar( this, SLOT( slotViewToolBar() ), actionCollection() );
    viewStatusBar = KStdAction::showStatusbar( this, SLOT( slotViewStatusBar() ), actionCollection() );

    gameServerConnect->setStatusText( i18n( "Connects to a server for a new game" ) );
    gameNewServer->setStatusText( i18n( "Opens a server game" ) );
    gameQuit->setStatusText( i18n( "Quits the application" ) );
    viewToolBar->setStatusText( i18n( "Enables/disables the toolbar" ) );
    viewStatusBar->setStatusText( i18n( "Enables/disables the statusbar" ) );
    configSound->setStatusText( i18n( "Enables/disables sound" ) );
    
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
    ownNickname = "-";
    enemyNickname = "-";
    statusBar()->insertItem( i18n( "     Player 1: %1     " ).arg( ownNickname ), ID_PLAYER_OWN, 0, true );
    statusBar()->insertItem( i18n( "     Player 2: %1     " ).arg( enemyNickname ), ID_PLAYER_ENEMY, 0, true );
    statusBar()->insertItem( i18n( "Ready" ), ID_STATUS_MSG, 1 );
    statusBar()->setItemAlignment( ID_STATUS_MSG, AlignLeft );
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

		if( stat->getHit() != 10 )
		{
		    sendMessage( fieldx, fieldy, showstate );
		    slotStatusMsg( i18n( "Waiting for enemy to shoot.." ) );
		}
		else
		{
		    sendMessage( fieldx, fieldy, showstate, true );
		    slotStatusMsg( i18n( "You won the game :)" ) );
		    if( connection->getType() == KonnectionHandling::SERVER )
			resetServer( true );
		    else
			resetClient( true );
		}
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

void KBattleshipApp::sendMessage( int fieldx, int fieldy, int state, bool won )
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
	if( won )
	    msg->addField( QString( "enemyM" ), QString( "won" ) );
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
    	msg->chatMessage( ownNickname, text );
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

void KBattleshipApp::clientRestart()
{
    slotStatusMsg( i18n( "Waiting for other player to place the ships..." ) );
    delete ownshiplist;
    delete enemyshiplist;
    ownshiplist = new KShipList();
    enemyshiplist = new KShipList();
}

void KBattleshipApp::saveOptions()
{
    config->setGroup( "General" );
    config->writeEntry( "ShowToolbar", viewToolBar->isChecked() );
    config->writeEntry( "ShowStatusbar", viewStatusBar->isChecked() );
    config->writeEntry( "PlaySounds", configSound->isChecked() );
    config->writeEntry( "ToolBarPos", (int) toolBar()->barPos() );
    config->sync();
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
    
    bool bConfigSound = config->readBoolEntry( "PlaySounds", true );
    configSound->setChecked( bConfigSound );
    slotConfigSound();

    KToolBar::BarPosition toolBarPos;
    toolBarPos = ( KToolBar::BarPosition ) config->readNumEntry( "ToolBarPos", KToolBar::Top );
    toolBar()->setBarPos( toolBarPos );
}

void KBattleshipApp::slotGameQuit()
{
    delete this;
    kapp->quit();
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

void KBattleshipApp::resetClient( bool status )
{
    if( status )
    {
    	KMessage *msg = new KMessage( KMessage::REPLAY );
	int retVal = KMessageBox::questionYesNo( this, "Do you want to ask to server restarting the game?" );
	switch( retVal )
	{
	    case KMessageBox::Yes:
	    	slotStatusMsg( i18n( "Waiting for other player to start the match..." ) );
    		view->clearField();
    		stat->clear();
		msg->addField( QString( "enemyW" ), QString( "replay" ) );
		if( !connection->writeable() )
		{
		    kbclient->allowWrite();
		    sendMessage( msg );
		    kbclient->forbidWrite();
		}
		break;
	
	    case KMessageBox::No:
		delete msg;
		slotStatusMsg( i18n( "Ready" ) );
		gameServerConnect->setText( "&Connect to server" );
		view->clearField();
		stat->clear();
		stoppedConnectDialog();
    		delete connection;
		delete kbclient;
    		connection = 0;
		break;
	}
    }
    else
    {
	slotStatusMsg( i18n( "Ready" ) );
	gameServerConnect->setText( "&Connect to server" );
	view->clearField();
	stat->clear();
	stoppedConnectDialog();
	delete connection;
	delete kbclient;
	connection = 0;
    }
    deleteLists();
}

void KBattleshipApp::askReplay()
{
    int retVal = KMessageBox::questionYesNo( this, "The client asks for restarting the game. Do you accept?" );
    switch( retVal )
    {
	case KMessageBox::Yes:
	    slotStatusMsg( i18n( "Please place your ships" ) );
	    view->clearField();
    	    stat->clear();
	    delete ownshiplist;
	    delete enemyshiplist;
	    ownshiplist = new KShipList();
	    enemyshiplist = new KShipList();
	    break;
	    
	case KMessageBox::No:
    	    slotStatusMsg( i18n( "Ready" ) );
	    gameNewServer->setText( "&Start server" );
	    view->clearField();
    	    stat->clear();
    	    stoppedServerDialog();
	    delete connection;
	    delete kbserver;
	    connection = 0;
	    delete ownshiplist;
	    delete enemyshiplist;
	    ownshiplist = new KShipList();
	    enemyshiplist = new KShipList();
	    break;
    }    
}

void KBattleshipApp::resetServer( bool status )
{
    if( status )
    {
    	KMessage *msg = new KMessage( KMessage::REPLAY );
	int retVal = KMessageBox::questionYesNo( this, "Do you want to restart the game?" );
	switch( retVal )
	{
	    case KMessageBox::Yes:
    		view->clearField();
    		stat->clear();
		msg->addField( QString( "enemyW" ), QString( "replay" ) );
		if( !connection->writeable() )
		{
		    kbserver->allowWrite();
		    sendMessage( msg );
		    kbserver->forbidWrite();
		}
		break;
	
	    case KMessageBox::No:
		delete msg;
	    	slotStatusMsg( i18n( "Ready" ) );
        	gameNewServer->setText( "&Start server" );
		view->clearField();
    		stat->clear();
	    	stoppedServerDialog();
	    	delete connection;
		delete kbserver;
	    	connection = 0;
		break;
	}
    }
    else
    {
    	slotStatusMsg( i18n( "Ready" ) );
        gameNewServer->setText( "&Start server" );
	view->clearField();
	stat->clear();
	stoppedServerDialog();
	delete connection;
	delete kbserver;
	connection = 0;
    }
    deleteLists();
}

void KBattleshipApp::deleteLists()
{
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

void KBattleshipApp::sendGreet()
{
    KMessage *msg = new KMessage( KMessage::GREET );
    msg->addField( QString( "nickname" ), ownNickname );
    sendMessage( msg );
}

void KBattleshipApp::startBattleshipServer()
{
    gameNewServer->setText( "&Stop server" );
    kbserver = new KBattleshipServer( ( server->getPort() ).toInt() );
    ownNickname = server->getNickname();
    chat->setNickname( ownNickname );
    slotChangeOwnPlayer( ownNickname );
    delete server;
    chat->acceptMsg( true );
    connection = new KonnectionHandling( this, kbserver );    
    connect( connection, SIGNAL( askReplay() ), this, SLOT( askReplay() ) );
    connect( connection, SIGNAL( abortGame() ), this, SLOT( deleteLists() ) );
    connect( connection, SIGNAL( serverFailure( bool ) ), this, SLOT( resetServer( bool ) ) );
    connect( connection, SIGNAL( giveEnemyName() ), this, SLOT( sendGreet() ) ); 
    connect( connection, SIGNAL( enemyNickname( const QString & ) ), this, SLOT( slotChangeEnemyPlayer( const QString & ) ) );
    connect( connection, SIGNAL( statusBarMessage( const QString & ) ), this, SLOT( slotStatusMsg( const QString & ) ) );
    connect( connection, SIGNAL( ownFieldDataChanged( int, int, int ) ), this, SLOT( changeOwnFieldData( int, int, int ) ) );
    connect( connection, SIGNAL( gotChatMessage( QString, QString ) ), chat, SLOT( receivedMessage( QString, QString ) ) );
    connect( connection, SIGNAL( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ), this, SLOT( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );
    kbserver->start();    
    slotStatusMsg( i18n( "Waiting for a player..." ) );
}

void KBattleshipApp::changeOwnFieldData( int fieldx, int fieldy, int type )
{
    view->changeOwnFieldData( fieldx, fieldy, type );
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
    view->changeEnemyFieldData( fieldx, fieldy, type );
    if( type == KBattleField::HIT )
    {
	if( enemyshiplist->getXYShipType( fieldx, fieldy ) != 0 )
	{
	    KShip *ship = enemyshiplist->getXYShip( fieldx, fieldy );
	    typedef QValueList<int> DeathValueList;
	    DeathValueList deathList;
    	    bool xokay = false, yokay = false;
	    int tempy = 0, tempx = 0;
	    int q = 0;
	    if( ship->shipystart() == ship->shipystop() && ship->shipxstart() != ship->shipxstop() )
    	    {
		for( tempx = ship->shipxstart(); tempx <= ship->shipxstop(); tempx++ )
		{
		    if( view->getEnemyFieldState( tempx, fieldy ) == KBattleField::HIT )
			deathList.append( tempx );
		    else
			xokay = false;
		
		    q = 0;
		    DeathValueList::Iterator it;
		    for( it = deathList.begin(); it != deathList.end(); it++ )
		    {
			q++;
			if( q - 1 == ship->shiptype() )
			    xokay = true;
			else
			    xokay = false;
		    }
		}
	    }
	    else if( ship->shipystart() != ship->shipystop() && ship->shipxstart() == ship->shipxstop() )
	    {
		for( tempy = ship->shipystart(); tempy <= ship->shipystop(); tempy++ )
		{
		    if( view->getEnemyFieldState( fieldx, tempy ) == KBattleField::HIT )
			deathList.append( tempy );
		    else
			yokay = false;

		    q = 0;
		    DeathValueList::Iterator it;
		    for( it = deathList.begin(); it != deathList.end(); it++ )
		    {
			q++;
			if( q - 1 == ship->shiptype() )
			    yokay = true;
			else
			    yokay = false;
		    }
		}
	    }
	
	    if( xokay && !yokay )
	    {
		DeathValueList::Iterator it;
		for( it = deathList.begin(); it != deathList.end(); it++ )
		    view->changeEnemyFieldData( *it, fieldy, 93 ); //KBattleField::DEATH ); AAARRRGL why isn't this working
	    }
	
	    if( !xokay && yokay )
	    {
		DeathValueList::Iterator it;
		for( it = deathList.begin(); it != deathList.end(); it++ )
	    	    view->changeEnemyFieldData( fieldx, *it, 93 ); //KBattleField::DEATH ); AAARRRGL why isn't this working
	    }
	}
	else
	    view->changeEnemyFieldData( fieldx, fieldy, 93 );
    }
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
	ownNickname = client->getNickname();
	chat->setNickname( ownNickname );
	slotChangeOwnPlayer( ownNickname );
	delete client;
        gameServerConnect->setText( "Dis&connect from server" );
	chat->acceptMsg( true );
        connection = new KonnectionHandling( this, kbclient );
	connect( connection, SIGNAL( clientRestart() ), this, SLOT( clientRestart() ) );
	connect( connection, SIGNAL( enemyNickname( const QString & ) ), this, SLOT( slotChangeEnemyPlayer( const QString & ) ) );
	connect( connection, SIGNAL( statusBarMessage( const QString & ) ), this, SLOT( slotStatusMsg( const QString & ) ) );
	connect( connection, SIGNAL( ownFieldDataChanged( int, int, int ) ), this, SLOT( changeOwnFieldData( int, int, int ) ) );
	connect( connection, SIGNAL( setPlaceable() ), this, SLOT( setPlaceable() ) );
	connect( connection, SIGNAL( abortGame() ), this, SLOT( deleteLists() ) );
	connect( connection, SIGNAL( abortGameStrict( bool ) ), this, SLOT( resetClient( bool ) ) );
        connect( connection, SIGNAL( changeConnectText() ), this, SLOT( changeConnectText() ) );
	connect( connection, SIGNAL( gotChatMessage( QString, QString ) ), chat, SLOT( receivedMessage( QString, QString ) ) );
	connect( connection, SIGNAL( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ), this, SLOT( gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );

	KMessage *msg = new KMessage( KMessage::GREET );
	msg->addField( QString( "nickname" ), ownNickname );
	sendMessage( msg );
	
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

void KBattleshipApp::slotConfigSound()
{
    if( !configSound->isChecked() )
        sound->turnOff();
    else
        sound->turnOn();
}

void KBattleshipApp::slotViewToolBar()
{
    if( !viewToolBar->isChecked() )
        toolBar()->hide();
    else
        toolBar()->show();
}

void KBattleshipApp::slotViewStatusBar()
{
    if( !viewStatusBar->isChecked() )
        statusBar()->hide();
    else
        statusBar()->show();
}

void KBattleshipApp::slotStatusMsg( const QString &text )
{
    statusBar()->clear();
    statusBar()->changeItem( text, ID_STATUS_MSG );
}

void KBattleshipApp::slotChangeOwnPlayer( const QString &text )
{
    statusBar()->clear();
    statusBar()->changeItem( i18n( "     Player 1: %1     " ).arg( text ), ID_PLAYER_OWN );
}

void KBattleshipApp::slotChangeEnemyPlayer( const QString &text )
{
    statusBar()->clear();
    statusBar()->changeItem( i18n( "     Player 2: %1     " ).arg( text ), ID_PLAYER_ENEMY );
}
