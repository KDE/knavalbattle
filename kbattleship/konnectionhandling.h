/***************************************************************************
                                 konnectionhandling.h
                                  -----------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>

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
        enum{SERVER, CLIENT};
        KonnectionHandling(QWidget *parent, KBattleshipServer *server);
        KonnectionHandling(QWidget *parent, KBattleshipClient *client);
        ~KonnectionHandling();

        int getType();
        int getFieldState();
        bool writeable();
        bool sendEnemyList();
        bool gotEnemyList();
        bool haveEnemy();
	
	void clear();

	void updateInternal(KBattleshipServer *server);
	void updateInternal(KBattleshipClient *client);
    
    public slots:
        void setEnemyList(bool set);
        void gotNewMessage(KMessage *msg);
        void serverError();
        void serverGotNewClient();
        void serverWroteToClient();
        void serverLostClient();	
        void clientLostServer();
        void clientSocketError(int error);
    
    signals:
	void updateHighscore();
        void newPlayer(bool);
        void clientRestart();
        void askReplay();
        void statusBarMessage(const QString &);
        void abortGame(bool);
        void abortGameStrict(bool);
        void setPlaceable();
        void gotEnemyShipList(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
        void gotChatMessage(QString, QString);
        void enemyNickname(const QString &);
        void ownFieldDataChanged(int, int, int);
        void changeConnectText();
        void giveEnemyName();
        void serverFailure(bool);
	void connected();

    private:
        KBattleshipServer *internalServer;
        KBattleshipClient *internalClient;
        bool enemylist;
        bool senemylist;
        bool enemy;
	bool m_showed;
        int internalType;
};

#endif
