/***************************************************************************
                                 konnectionhandling.h
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
 
#ifndef KONNECTIONHANDLING_H
#define KONNECTIONHANDLING_H

#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qobject.h>
#include <qwidget.h>
#include "kmessage.h"
#include "kbattlefield.h"
#include "kbattleshipserver.h"
#include "kbattleshipclient.h"

class KonnectionHandling : public QObject
{
    Q_OBJECT
    public:
	enum{ SERVER, CLIENT };
        KonnectionHandling( QWidget *parent, KBattleshipServer *server );
        KonnectionHandling( QWidget *parent, KBattleshipClient *client );
        ~KonnectionHandling();
	
	int getType();
	int getFieldState();
	bool writeable();
	bool sendEnemyList();
	bool gotEnemyList();
	bool haveEnemy();
	
    public slots:
	void setEnemyList( bool set );
	void gotNewMessage( KMessage *msg );
        void serverGotNewClient();
	void serverWroteToClient();
	void serverLostClient();	
	void clientLostServer();
	void clientSocketError( int error );
	
    private:
	KBattleshipServer *internalServer;
	KBattleshipClient *internalClient;
	bool enemylist;
	bool senemylist;
	bool enemy;
	int internalType;

    signals:	
	void statusBarMessage( const QString & );
	void abortGame();
	void setPlaceable();
	void gotEnemyShipList( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString );
	void gotChatMessage( QString, QString );
	void enemyNickname( QString );
	void ownFieldDataChanged( int, int, int );
	void changeConnectText();
	void giveEnemyName();
	void serverFailure();
};

#endif

