/***************************************************************************
                                   kbattleship.h
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

#ifndef KBATTLESHIP_H
#define KBATTLESHIP_H

#include <stdlib.h>
#include <kapp.h>
#include <kmainwindow.h>
#include <kaccel.h>
#include <kstdaction.h>
#include <kaction.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <qpainter.h>
#include <qobject.h>
#include <qstring.h>
#include <arts/dispatcher.h>
#include <arts/soundserver.h>

#include "kbattleship.h"
#include "kbattleshipview.h"
#include "kbattleshipsound.h"
#include "kbattleshipclient.h"
#include "kbattleshipserver.h"
#include "konnectionhandling.h"
#include "kserverdialog.h"
#include "kclientdialog.h"

using namespace std;
using namespace Arts;

class KBattleshipView;

class KBattleshipApp : public KMainWindow
{
    Q_OBJECT

    public:
        KBattleshipApp( QWidget *parent = 0, const char *name = 0 );
        ~KBattleshipApp();
	
    protected:
        void saveOptions();
        void readOptions();
        void initActions();
        void initStatusBar();
        void initDocument();
        void initView();
	void initSound();

    public slots:
	void slotServerConnect();
	void slotNewServer();
        void slotGameQuit();
        void slotViewToolBar();
        void slotViewStatusBar();
        void slotStatusMsg( const QString &text );
	void startBattleshipServer();
	void connectToBattleshipServer();
	void sendMessage( int fieldX, int fieldY );
	void changeOwnFieldData( int fieldX, int fieldY, int type );
	void changeConnectText( QString text );

    private:
        KConfig *config;
        KBattleshipView *view;
	KBattleshipServer *kbserver;
	KBattleshipClient *kbclient;
	KAction *gameServerConnect;
	KAction *gameNewServer;
        KAction *gameQuit;
        KToggleAction *viewToolBar;
        KToggleAction *viewStatusBar;
	KBattleshipSound *sound;
        KClientDialog *client;
	KServerDialog *server;
	KonnectionHandling *connection;
	bool haveCS;
};

#endif
