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

#define ID_STATUS_MSG 1

KBattleshipApp::KBattleshipApp( QWidget *, const char *name ) : KMainWindow( 0, name )
{
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
    KBattleshipSound *soundserver = new KBattleshipSound();
}

void KBattleshipApp::initStatusBar()
{
    statusBar()->insertItem( i18n( "Ready." ), ID_STATUS_MSG );
}

void KBattleshipApp::initView()
{
    view = new KBattleshipView( this );
    setCentralWidget( view );
    view->startDrawing();
    setCaption( i18n( "KBattleship (pre-alpha)" ), false );
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
    toolBarPos = (KToolBar::BarPosition) config->readNumEntry( "ToolBarPos", KToolBar::Top );
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

void KBattleshipApp::slotServerConnect()
{
    if( !connection)
    {
        slotStatusMsg( i18n( "Loading Connect-Server dialog..." ) );

	// TODO: use KDialogBase

        client = new KClientDialog();
        connect( client, SIGNAL( connectServer() ), this, SLOT( connectToBattleshipServer() ) );
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
		delete connection;
		delete kbclient;
		connection = 0;
		break;
	}
	    
    }
}

void KBattleshipApp::slotNewServer()
{
    if( !connection )
    {
        slotStatusMsg( i18n( "Loading Start-Server dialog..." ) );
    
	// TODO: use KDialogBase
    
        server = new KServerDialog();
	connect( server, SIGNAL( startServer() ), this, SLOT( startBattleshipServer() ) );
        server->show();
            
	slotStatusMsg( i18n( "Ready." ) );
    }
    else
    {
	switch( connection->getType() )
	{
	    case KonnectionHandling::SERVER:
	        gameNewServer->setText( "&Start server" );
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
    kbserver = new KBattleshipServer( ( server->getPort() ).toInt() );
    delete server;
    gameNewServer->setText( "&Stop server" );
    connection = new KonnectionHandling( this, kbserver );    
    connect( connection, SIGNAL( ownFieldDataChanged( int, int, int ) ), view, SLOT( changeOwnFieldData( int, int, int ) ) );
}

void KBattleshipApp::connectToBattleshipServer()
{
    if( client->getHost() != "" )
    {
	kbclient = new KBattleshipClient( client->getHost(), ( client->getPort() ).toInt() );
        delete client;
        gameServerConnect->setText( "Dis&connect from server" );
        connection = new KonnectionHandling( this, kbclient );
    }
    else
    {
	KMessageBox::error( this, i18n( "You forgot to enter a host!" ) );
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

