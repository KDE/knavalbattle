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
	
    public slots:
	void gotBattleFieldState( int fieldx, int fieldy, int state );
	void gotNewMessage( KMessage *msg );
        void serverGotNewClient();
	void serverWroteToClient();
	void serverLostClient();	
	void clientLostServer();
	void clientSocketError( int error );
	
    private:
	int internalType;

    signals:	
	void gotChatMessage( QString *nickname, QString *msg );
	void requestBattleFieldState( int fieldx, int fieldy );
	void ownFieldDataChanged( int fieldx, int fieldy, int type );
	void enemyFieldDataChanged( int fieldx, int fieldy, int type );
	void changeConnectText();
	void sendMessage( KMessage *msg );
};

#endif

