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
#include <qsplitter.h>
#include <arts/dispatcher.h>
#include <arts/soundserver.h>

#include "kbattleship.h"
#include "kbattleshipview.h"
#include "kbattleshipsound.h"
#include "kbattleshipclient.h"
#include "kbattleshipserver.h"
#include "konnectionhandling.h"
#include "kshiplist.h"
#include "kserverdialog.h"
#include "kclientdialog.h"
#include "kstatdialog.h"
#include "kchatwidget.h"

class KBattleshipView;

class KBattleshipApp : public KMainWindow
{
    Q_OBJECT
    public:
        KBattleshipApp( QWidget *parent = 0, const char *name = 0 );
        ~KBattleshipApp();

	void init();

    protected:
        void saveOptions();
        void readOptions();
        void initActions();
        void initStatusBar();
        void initView();
	void initChat();
	void initSound();
	void initShipPlacing();

    public slots:
	void sendGreet();
	void resetClient();
	void resetServer();
	void setPlaceable();
	void slotServerConnect();
	void slotNewServer();
        void slotGameQuit();
	void slotConfigSound();
        void slotViewToolBar();
        void slotViewStatusBar();
        void slotStatusMsg( const QString &text );
        void slotChangeOwnPlayer( const QString &text );	
        void slotChangeEnemyPlayer( const QString &text );
	void startBattleshipServer();
	void connectToBattleshipServer();
	void placeShip( int fieldx, int fieldy, int button );
	void enemyClick( int fieldx, int fieldy );
	void sendMessage( int fieldx, int fieldy, int state );
	void sendMessage( KMessage *msg );
	void sendShipList();
	void sendChatMessage( QString text );
	void changeOwnFieldData( int fieldx, int fieldy, int type );
	void changeEnemyFieldData( int fieldx, int fieldy, int type );
	void changeConnectText();
	void changeStartText();
	void stoppedServerDialog();
	void stoppedConnectDialog();
	void requestedOwnFieldShipListJob( int fieldx, int fieldy, QPainter *painter, bool hit, bool death );
	void requestedEnemyFieldShipListJob( int fieldx, int fieldy, QPainter *painter );
	void gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString );

    private:
        KConfig *config;
        QSplitter *splitH;
        QSplitter *splitV;
        KChatWidget *chat;
	KStatDialog *stat;
        KBattleshipView *view;
	KBattleshipServer *kbserver;
	KBattleshipClient *kbclient;
	KAction *gameServerConnect;
	KAction *gameNewServer;
        KAction *gameQuit;
        KToggleAction *viewToolBar;
        KToggleAction *viewStatusBar;
	KToggleAction *configSound;
	KBattleshipSound *sound;
        KClientDialog *client;
	KServerDialog *server;
	KonnectionHandling *connection;
	KShipList *ownshiplist;
	KShipList *enemyshiplist;
	bool haveCS;
	bool place;
	QString ownNickname;
	QString enemyNickname;
    	
    signals:
	void battleFieldState( int, int, int );
};

#endif
