/***************************************************************************
                                  kbattleship.cpp
                                  -----------------
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

#include "kbattleship.moc"

#define ID_STATUS_MSG 1

KBattleshipApp::KBattleshipApp( QWidget *, const char *name ) : KMainWindow( 0, name )
{
    config = kapp->config();
    initStatusBar();
    initActions();
    initView();
    readOptions();
}

KBattleshipApp::~KBattleshipApp()
{
}

void KBattleshipApp::initActions()
{
    fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
    viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
    viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());

    fileQuit->setStatusText( i18n( "Quits the application" ) );
    viewToolBar->setStatusText( i18n( "Enables/disables the toolbar" ) );
    viewStatusBar->setStatusText( i18n( "Enables/disables the statusbar" ) );

    createGUI();
}


void KBattleshipApp::initStatusBar()
{
    statusBar()->insertItem( i18n("Ready."), ID_STATUS_MSG );
}

void KBattleshipApp::initView()
{
    view = new KBattleshipView( this );
    setCentralWidget( view );
    setCaption( i18n( "KBattleship (pre-alpha)" ), false );
}

void KBattleshipApp::saveOptions()
{
    config->setGroup( "General" );
    config->writeEntry( "AppGeometry", size() );
    config->writeEntry( "ShowToolbar", viewToolBar->isChecked() );
    config->writeEntry( "ShowStatusbar", viewStatusBar->isChecked() );
    config->writeEntry( "ToolBarPos", (int) toolBar( "mainToolBar" )->barPos() );
}


void KBattleshipApp::readOptions()
{
    config->setGroup("General");

    bool bViewToolbar = config->readBoolEntry( "ShowToolbar", true );
    viewToolBar->setChecked( bViewToolbar );
    slotViewToolBar();

    bool bViewStatusbar = config->readBoolEntry( "Show Statusbar", true );
    viewStatusBar->setChecked( bViewStatusbar );
    slotViewStatusBar();

    KToolBar::BarPosition toolBarPos;
    toolBarPos = (KToolBar::BarPosition) config->readNumEntry( "ToolBarPos", KToolBar::Top );
    toolBar( "mainToolBar" )->setBarPos( toolBarPos );

    QSize size = config->readSizeEntry( "Geometry" );
    if( !size.isEmpty() )
    {
        resize( size );
    }
}

bool KBattleshipApp::queryExit()
{
    saveOptions();
    return true;
}

void KBattleshipApp::slotFileQuit()
{
    slotStatusMsg( i18n( "Exiting..." ) );
    saveOptions();

    KMainWindow *window;
    if( memberList )
    {
        for( window = memberList->first(); window != 0; window = memberList->first() )
        {
            if( !w->close() )
                break;
        }
    }

    slotStatusMsg( i18n( "Ready." ) );
}

void KBattleshipApp::slotViewToolBar()
{
    slotStatusMsg(i18n("Toggling toolbar..."));
    if( !viewToolBar->isChecked() )
    {
        toolBar( "mainToolBar" )->hide();
    }
    else
    {
        toolBar( "mainToolBar" )->show();
    }

    slotStatusMsg( i18n( "Ready." ) );
}

void KBattleshipApp::slotViewStatusBar()
{
    slotStatusMsg( i18n( "Toggle the statusbar..." ) );
    if( !viewStatusBar->isChecked() )
    {
        statusBar()->hide();
    }
    else
    {
        statusBar()->show();
    }

    slotStatusMsg(i18n("Ready."));
}


void KBattleshipApp::slotStatusMsg( const QString &text )
{
    statusBar()->clear();
    statusBar()->changeItem( text, ID_STATUS_MSG );
}

