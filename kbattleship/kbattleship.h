/***************************************************************************
                                    kbattleship.h
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

#ifndef KBATTLESHIP_H
#define KBATTLESHIP_H

#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kstdaction.h>

#include <qstring.h>

#include "dialogs/infoDlg.h"

#include "kbaiplayer.h"
#include "kbattleship.h"
#include "kbattleshipclient.h"
#include "kbattleshipserver.h"
#include "kbattleshipview.h"
#include "kchatwidget.h"
#include "kclientdialog.h"
#include "konnectionhandling.h"
#include "kserverdialog.h"
#include "kship.h"
#include "kshiplist.h"
#include "kstatdialog.h"

class KBattleshipApp : public KMainWindow
{
	Q_OBJECT
public:
	enum{ID_STATUS_MSG, ID_PLAYER_OWN, ID_PLAYER_ENEMY};
	KBattleshipApp();
	~KBattleshipApp();

	void init();

	KShip *shipAt(int fieldx, int fieldy);
	KShip *enemyShipAt(int fieldx, int fieldy);

private slots:
	void slotConfigureKeys();
	void slotConfigureNotifications();
	void slotLost(KMessage *msg);
	void slotStatusMsg(const QString &text);
	void slotReceivedEnemyFieldData(int fieldx, int fieldx1, int enemystate, int xstart, int xstop, int ystart, int ystop, bool death);
	void slotSendEnemyFieldState(int, int);
	void slotChangeOwnPlayer(const QString &text);
	void slotChangeEnemyPlayer(const QString &text);
	void slotSendVersion();
	void slotSendGreet();
	void slotShipsReady();
	void slotSetPlaceable(bool place);
	void slotSetShootable(bool shoot);
	void slotEnemyFieldClick(int fieldx, int fieldy);
	void slotSendMessage(int fieldx, int fieldy, int state);
	void slotSendMessage(KMessage *msg);
	void slotClientLost();
	void slotServerLost();
	void slotServerReplay();
	void slotClientReplay();
	void slotAIReady();
	void slotAIShootsAt(const QPoint pos);
	void slotDeleteAI();
	void slotRestartAI();
	void slotSinglePlayer();
	void slotServerConnect();
	void slotDeleteConnectDialog();
	void slotNewServer();
	void slotDeleteServerDialog();
	void slotHighscore();
	void slotShowGrid();
	void slotStartBattleshipGame();
	void slotStartBattleshipGame(bool clearstat);
	void slotStartBattleshipServer();
	void slotConnectToBattleshipServer();
	void slotPlaceShipPreview(int fieldx, int fieldy, bool shift);
	void slotPlaceShip(int fieldx, int fieldy, int button);
	void slotChangeOwnFieldData(int fieldx, int fieldy, int type);
	void slotChangeEnemyFieldData(int fieldx, int fieldy, int type);
	void slotUpdateHighscore();
	void slotAbortNetworkGame();
	void slotReplay();
	void slotReplayRequest();
	void slotChangedNickCommand(const QString &text);
	void slotSendChatMessage(const QString &text);
	void slotEnemyClientInfo();
	void slotReceivedClientInformation(const QString &client, const QString &clientVersion, const QString &clientInformation, const QString &protocolVersion);

private:
	void initActions();
	void initStatusBar();
	void initView();
	void initChat();
	void initShipPlacing();
	void saveOptions();
	void readOptions();

	void cleanup(bool placechange = true);
	void playSound(bool enemy, int fieldstate);

	bool m_placeable;
	bool m_shootable;
	bool m_aiPlaying;
	bool m_serverHasClient;
	bool m_lost;
	int m_aiHits;

	QString m_enemyClient;
	QString m_enemyClientVersion;
	QString m_enemyClientDescription;
	QString m_enemyProtocolVersion;

	KConfig *m_config;
	KBAIPlayer *m_aiPlayer;
	KonnectionHandling *m_connection;
	KBattleshipServer *m_kbserver;
	KBattleshipClient *m_kbclient;
	KChatWidget *m_chat;
	KStatDialog *m_stat;
	KBattleshipView *m_view;
	KAction *m_gameServerConnect;
	KAction *m_gameNewServer;
	KAction *m_gameQuit;
	KAction *m_gameEnemyInfo;
	KAction *m_gameSingle;
	KToggleAction *m_configSound;
	KToggleAction *m_configGrid;
	KClientDialog *m_client;
	KServerDialog *m_server;
	KShipList *m_ownshiplist;
	KShipList *m_enemyshiplist;
	QString m_ownNickname;
	QString m_enemyNickname;

};
#endif
