/***************************************************************************
                                  kbattleship.cpp
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

#include <qtimer.h>
#include <stdlib.h>
#include <qlayout.h>
#include <kgamemisc.h>
#include "main.h"
#include <kstatusbar.h>
#include "kbattleship.moc"

KBattleshipApp::KBattleshipApp(QWidget *, const char *name) : KMainWindow(0, name)
{
	setMinimumSize(750, 500);
	m_connection = 0;
	m_lost = 0;
	m_config = 0;
	m_client = 0;
	m_server = 0;
	m_single = 0;
	m_aiPlayer = 0;
	m_aiHits = 0;

	QString picDirCheck = locate("data", "kbattleship/pictures/");
	QString onePicCheck = locate("data", "kbattleship/pictures/hit.png");
	if(picDirCheck.isEmpty() || onePicCheck.isEmpty())
	{
		KMessageBox::error(this, i18n("You don't have KBattleship Pictures installed. The game cannot run without them!"));
		exit(0);
	}
	else
		init();
}

KBattleshipApp::~KBattleshipApp()
{
	if(m_config != 0)
		saveOptions();
	delete m_aiPlayer;
	delete m_ownshiplist;
	delete m_enemyshiplist;
	delete m_sound;
}

void KBattleshipApp::init()
{
	m_aiPlaying = false;
	m_placeable = false;
	m_shootable = false;
	m_serverHasClient = false;
	m_config = kapp->config();
	initStatusBar();
	initActions();
	readOptions();
	initView();
	initChat();
	initSound();
	initShipPlacing();
}

void KBattleshipApp::initStatusBar()
{
	m_ownNickname = "-";
	m_enemyNickname = "-";
	statusBar()->insertItem(i18n("     Player 1: %1     ").arg(m_ownNickname), ID_PLAYER_OWN, 0, true);
	statusBar()->insertItem(i18n("     Player 2: %1     ").arg(m_enemyNickname), ID_PLAYER_ENEMY, 0, true);
	statusBar()->insertItem(i18n("Ready"), ID_STATUS_MSG, 1);
	statusBar()->setItemAlignment(ID_STATUS_MSG, AlignLeft);
}

void KBattleshipApp::initActions()
{
	m_gameServerConnect = new KAction(i18n("&Connect to Server..."), "connect_no", Key_F2, this, SLOT(slotServerConnect()), actionCollection(), "serverconnect");
	m_gameNewServer = new KAction(i18n("&Start Server..."), "network", Key_F3, this, SLOT(slotNewServer()), actionCollection(), "newserver");
	m_gameSingle = new KAction(i18n("S&ingle Player..."), "gear", Key_F4, this, SLOT(slotSinglePlayer()), actionCollection(), "singleplayer");
	m_gameQuit = KStdAction::quit( this, SLOT(slotGameQuit()), actionCollection(), "gamequit");
	(void) new KAction(i18n("&Highscore"), "view_text", Key_F10, this, SLOT(slotHighscore()), actionCollection(), "highscore");
	m_gameEnemyInfo = new KAction(i18n("&Enemy Info..."), "view_text", Key_F11, this, SLOT(slotEnemyClientInfo()), actionCollection(), "enemyinfo");

	m_viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	m_configSound = new KToggleAction(i18n("&Play Sounds"), 0, this, SLOT(slotConfigSound()), actionCollection(), "configsound");
	m_configGrid = new KToggleAction(i18n("&Show Grid"), 0, this, SLOT(slotShowGrid()), actionCollection(), "showgrid");

	m_gameEnemyInfo->setEnabled(false);
	createGUI();
}

void KBattleshipApp::initSound()
{
	new Arts::Dispatcher;
	m_sound = new KBattleshipSound();

	if(!m_sound->initSoundServer())
		m_configSound->setChecked(false);
	else
		slotConfigSound();
}

void KBattleshipApp::initChat()
{
	connect(m_chat, SIGNAL(sigSendMessage(const QString &)), this, SLOT(slotSendChatMessage(const QString &)));
	connect(m_chat, SIGNAL(sigChangeEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
	connect(m_chat, SIGNAL(sigChangeOwnNickname(const QString &)), this, SLOT(slotChangedNickCommand(const QString &)));
}

void KBattleshipApp::initShipPlacing()
{
	connect(m_ownshiplist, SIGNAL(sigOwnFieldDataChanged(int, int, int)), this, SLOT(slotChangeOwnFieldData(int, int, int)));
	connect(m_ownshiplist, SIGNAL(sigLastShipAdded()), this, SLOT(slotShipsReady()));
}

void KBattleshipApp::initView()
{
	QWidget *dummy = new QWidget(this, "dummy");
	setCentralWidget(dummy);

	QGridLayout *topLayout = new QGridLayout(dummy, 2, 2, 0, -1, "topLayout");

	m_chat = new KChatWidget(dummy);
	m_view = new KBattleshipView(dummy, "", m_configGrid->isChecked());
	m_stat = new KStatDialog(dummy);
	topLayout->setColStretch(1, 10);
	topLayout->setRowStretch(1, 10);
	topLayout->addWidget(m_view, 0, 0);
	topLayout->addWidget(m_stat, 0, 1);
	topLayout->addMultiCellWidget(m_chat, 1, 1, 0, 1);

	m_ownshiplist = new KShipList();
	m_enemyshiplist = new KShipList();
	m_score = new KHighscoreDialog(this);

	m_view->startDrawing();
	setFocusProxy(m_view);

	connect(m_view, SIGNAL(sigEnemyFieldClicked(int, int)), this, SLOT(slotEnemyFieldClick(int, int)));
	connect(m_view, SIGNAL(sigOwnFieldClicked(int, int, int)), this, SLOT(slotPlaceShip(int, int, int)));
	connect(m_view, SIGNAL(sigMouseOverField(int, int, bool)), this, SLOT(slotPlaceShipPreview(int, int, bool)));

	setCaption(i18n("KBattleship %1").arg(clientVersion), false);
}

void KBattleshipApp::slotDeleteAI()
{
	m_aiHits = 0;
	delete m_aiPlayer;
	m_aiPlayer = 0;
}

void KBattleshipApp::slotRestartAI()
{
	m_aiHits = 0;
	slotStartBattleshipGame(false);
}

void KBattleshipApp::slotEnemyFieldClick(int fieldx, int fieldy)
{
	if(m_connection != 0 || m_aiPlaying)
	{
		if(!m_aiPlaying && m_connection == 0)
			return;

		if(!m_serverHasClient && m_connection != 0)
			return;

		if(!m_shootable)
			return;

		if(m_view->enemyFieldState(fieldx, fieldy) == KBattleField::FREE)
		{
			if(!m_aiPlaying && !m_lost)
			{
				slotStatusMsg(i18n("Sending Message..."));
				KMessage *msg = new KMessage(KMessage::SHOOT);
				msg->addField("fieldx", QString::number(fieldx));
				msg->addField("fieldy", QString::number(fieldy));
				slotSendMessage(msg);
			}

			if(m_stat->hit() != 10 && m_aiPlaying)
			{
				m_stat->setShot();

				int showstate;

				if(m_enemyshiplist->shipTypeAt(fieldx, fieldy) == 99)
				{
					m_stat->setWater();
					showstate = KBattleField::WATER;
				}
				else
				{
					m_stat->setHit();
					showstate = KBattleField::HIT;
				}

				slotChangeEnemyFieldData(fieldx, fieldy, showstate);

				if(showstate == KBattleField::HIT)
				{
					if(m_enemyshiplist->shipTypeAt(fieldx, fieldy) != 0 && m_enemyshiplist->shipTypeAt(fieldx, fieldy) != 99)
					{
						KShip *ship = m_enemyshiplist->shipAt(fieldx, fieldy);
						typedef QValueList<int> DeathValueList;
						DeathValueList deathList;
						bool xokay = true, yokay = true;
						int tempy = 0, tempx = 0;

						if(ship->shipystart() == ship->shipystop() && ship->shipxstart() != ship->shipxstop())
						{
							for(tempx = ship->shipxstart(); tempx <= ship->shipxstop(); tempx++)
							{
								if(m_view->enemyFieldState(tempx, fieldy) == KBattleField::HIT)
								{
									deathList.append(tempx);
									xokay = true;
									yokay = false;
								}
								else
								{
									xokay = false;
									yokay = false;
									break;
								}
							}
						}
						else if(ship->shipystart() != ship->shipystop() && ship->shipxstart() == ship->shipxstop())
						{
							for(tempy = ship->shipystart(); tempy <= ship->shipystop(); tempy++)
							{
								if(m_view->enemyFieldState(fieldx, tempy) == KBattleField::HIT)
								{
									deathList.append(tempy);
									xokay = false;
									yokay = true;
								}
								else
								{
									xokay = false;
									yokay = false;
									break;
								}
							}
						}

						if(xokay)
						{
							DeathValueList::Iterator it;
							for(it = deathList.begin(); it != deathList.end(); it++){
								if(fieldy+1<m_enemyshiplist->m_fieldy) m_view->changeEnemyFieldData(*it, fieldy+1, KBattleField::BORDER);
								m_view->changeEnemyFieldData(*it, fieldy, KBattleField::DEATH);
								if(fieldy>0) m_view->changeEnemyFieldData(*it, fieldy-1, KBattleField::BORDER);
								}
							if(ship->shipxstart()>0){
							if (fieldy>0)m_view->changeEnemyFieldData(ship->shipxstart()-1, fieldy-1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(ship->shipxstart()-1, fieldy, KBattleField::BORDER);
							if (fieldy<m_enemyshiplist->m_fieldy)m_view->changeEnemyFieldData(ship->shipxstart()-1, fieldy+1, KBattleField::BORDER);
							}
							if(ship->shipxstop()<m_enemyshiplist->m_fieldx){
							if (fieldy>0)m_view->changeEnemyFieldData(ship->shipxstop()+1, fieldy-1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(ship->shipxstop()+1, fieldy, KBattleField::BORDER);
							if (fieldy<m_enemyshiplist->m_fieldy)m_view->changeEnemyFieldData(ship->shipxstop()+1,fieldy+1, KBattleField::BORDER);
							}

						}
						else if(yokay)
						{
							DeathValueList::Iterator it;
							for(it = deathList.begin(); it != deathList.end(); it++){
								if (fieldx>0) m_view->changeEnemyFieldData(fieldx-1, *it, KBattleField::BORDER);
								m_view->changeEnemyFieldData(fieldx, *it, KBattleField::DEATH);
								if(fieldx<m_enemyshiplist->m_fieldx) m_view->changeEnemyFieldData(fieldx+1, *it, KBattleField::BORDER);
							}
							if(ship->shipystart()>0){
							if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, ship->shipystart()-1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(fieldx, ship->shipystart()-1, KBattleField::BORDER);
							if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, ship->shipystart()-1, KBattleField::BORDER);
							}
							if(ship->shipystop()<m_enemyshiplist->m_fieldy){
							if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, ship->shipystop()+1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(fieldx, ship->shipystop()+1, KBattleField::BORDER);
							if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, ship->shipystop()+1, KBattleField::BORDER);
							}
						}
					}
					else if(m_enemyshiplist->shipTypeAt(fieldx, fieldy) == 0){
						m_view->changeEnemyFieldData(fieldx, fieldy, KBattleField::DEATH);
							if (fieldx>0) m_view->changeEnemyFieldData(fieldx-1, fieldy, KBattleField::BORDER);
							if(fieldx<m_enemyshiplist->m_fieldx) m_view->changeEnemyFieldData(fieldx+1, fieldy, KBattleField::BORDER);
							if(fieldy>1){
							if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, fieldy-1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(fieldx, fieldy-1, KBattleField::BORDER);
							if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, fieldy-1, KBattleField::BORDER);
							}
							if(fieldy<m_enemyshiplist->m_fieldy){
							if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, fieldy+1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(fieldx, fieldy+1, KBattleField::BORDER);
							if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, fieldy+1, KBattleField::BORDER);
							}
						}
				}
			}

			if(m_stat->hit() == 10 && m_aiPlaying)
			{
				m_aiPlaying = false;
				m_shootable = false;
				slotChangeOwnPlayer("-");
				slotChangeEnemyPlayer("-");
				m_gameSingle->setText(i18n("S&ingle Player"));
				m_gameNewServer->setEnabled(true);
				m_gameServerConnect->setEnabled(true);
				slotStatusMsg(i18n("You won the game :)"));
				m_stat->slotAddOwnWon();
				slotUpdateHighscore();
				switch(KMessageBox::questionYesNo(this, i18n("Do you want to restart the game?")))
				{
					case KMessageBox::Yes:
						QTimer::singleShot(0, this, SLOT(slotRestartAI()));
						break;

					case KMessageBox::No:
						QTimer::singleShot(0, this, SLOT(slotDeleteAI()));
						break;
				}
				return;
			}
			else if(m_aiPlaying && m_aiPlayer != 0)
				m_aiPlayer->slotRequestShot();
		}
	}
}

void KBattleshipApp::slotReceivedEnemyFieldData(int fieldx, int fieldy, int enemystate, int xstart, int xstop, int ystart, int ystop, bool death)
{
	m_stat->setShot();

	int showstate;

	if(enemystate == 99)
	{
		m_stat->setWater();
		showstate = KBattleField::WATER;
	}
	else
	{
		m_stat->setHit();
		showstate = KBattleField::HIT;
	}

	slotChangeEnemyFieldData(fieldx, fieldy, showstate);

	if(death)
	{
		if(xstart == xstop)
		{
			for(int i = ystart; i <= ystop; i++)
				slotChangeEnemyFieldData(fieldx, i, KBattleField::DEATH);
		}
		else if(ystart == ystop)
		{
			for(int i = xstart; i <= xstop; i++)
				slotChangeEnemyFieldData(i, fieldy, KBattleField::DEATH);
		}
	}

	if(m_stat->hit() != 10)
		slotStatusMsg(i18n("Waiting for enemy to shoot.."));
	else
	{
		KMessage *msg = new KMessage(KMessage::WON);
		slotSendMessage(msg);
		slotStatusMsg(i18n("You won the game :)"));
		m_stat->slotAddOwnWon();
		slotUpdateHighscore();
		if(m_connection->type() == KonnectionHandling::SERVER)
			slotServerReplay();
		else
			slotClientReplay();
	}
}

void KBattleshipApp::slotClientLost()
{
	slotAbortNetworkGame();
	slotStatusMsg(i18n("Enemy disconnected."));
}

void KBattleshipApp::slotServerLost()
{
	slotAbortNetworkGame();
	slotStatusMsg(i18n("Enemy disconnected."));
}

void KBattleshipApp::slotAbortNetworkGame()
{
	slotStatusMsg(i18n("Ready"));
	slotChangeOwnPlayer("-");
	slotChangeEnemyPlayer("-");

	m_gameServerConnect->setText(i18n("&Connect to server"));
	m_gameNewServer->setText(i18n("&Start server"));
	m_gameSingle->setText(i18n("S&ingle game"));
	m_gameServerConnect->setEnabled(true);
	m_gameNewServer->setEnabled(true);
	m_gameSingle->setEnabled(true);
	m_gameEnemyInfo->setEnabled(false);

	m_chat->clear();

	m_aiPlaying = false;
	m_shootable = false;
	m_placeable = false;
	m_serverHasClient = false;

	if(m_connection->type() == KonnectionHandling::SERVER)
	{
		delete m_kbserver;
		m_kbserver = 0;
	}
	else
		QTimer::singleShot(0, this, SLOT(slotDeleteClient()));

	delete m_connection;
	m_connection = 0;
}

void KBattleshipApp::slotReplay()
{
	cleanup(true);
	m_aiPlaying = false;
	m_shootable = false;
	m_lost = false;
	if(m_connection->type() == KonnectionHandling::SERVER)
		m_placeable = true;
	else
		m_placeable = false;
	m_stat->clear();
}

void KBattleshipApp::slotPlaceShipPreview(int fieldx, int fieldy, bool shift)
{
	int xadd = 0, yadd = 0;

	if(m_connection != 0 || m_aiPlaying)
	{
		if(!m_aiPlaying && m_connection == 0)
			return;

		if(m_connection != 0 && !m_aiPlaying && !m_serverHasClient)
			return;

		if((m_placeable && m_ownshiplist->canAddShips()) || m_aiPlaying)
		{
			switch(m_ownshiplist->shipCount())
			{
				case 4:
					for(int i = 0; i <= 3; i++)
					{
						if(!shift)
							xadd = i;
						else
							yadd = i;
						m_view->previewShip(fieldx + xadd, fieldy + yadd, KBattleField::SHIP4P1 + i, shift);
					}
					break;

				case 3:
					for(int i = 0; i <= 2; i++)
					{
						if(!shift)
							xadd = i;
						else
							yadd = i;
						m_view->previewShip(fieldx + xadd, fieldy + yadd, KBattleField::SHIP3P1 + i, shift);
					}
					break;

				case 2:
					for(int i = 0; i <= 1; i++)
					{
						if(!shift)
							xadd = i;
						else
							yadd = i;
						m_view->previewShip(fieldx + xadd, fieldy + yadd, KBattleField::SHIP2P1 + i, shift);
					}
					break;

				case 1:
					m_view->previewShip(fieldx, fieldy, KBattleField::SHIP1P1, shift);
					break;
			}

			m_view->field()->drawField();
		}
	}
}

void KBattleshipApp::slotPlaceShip(int fieldx, int fieldy, int button)
{
	if(m_connection != 0 || m_aiPlaying)
	{
		if(!m_aiPlaying && m_connection == 0)
			return;

		if(m_connection != 0 && !m_aiPlaying && !m_serverHasClient)
			return;

		if(m_placeable && m_ownshiplist->canAddShips())
			m_ownshiplist->addNewShip(button, fieldx, fieldy);
	}
}

void KBattleshipApp::slotShipsReady()
{
	if(m_aiPlaying)
	{
		slotStatusMsg(i18n("Waiting for computer player to start the match..."));
		m_placeable = false;
		m_aiPlayer->slotRequestShot();
		return;
	}

	KMessage *msg = new KMessage(KMessage::SHIPSREADY);
	slotSendMessage(msg);

	if(m_connection->type() == KonnectionHandling::SERVER)
		slotStatusMsg(i18n("Waiting for other player to place the ships..."));
	else
		slotStatusMsg(i18n("Waiting for other player to start the match..."));

	m_placeable = false;
}

void KBattleshipApp::slotSendMessage(int fieldx, int fieldy, int state)
{
	if(m_connection != 0)
	{
		KMessage *msg = new KMessage(KMessage::ANSWER_SHOOT);
		msg->addField(QString("fieldx"), QString::number(fieldx));
		msg->addField(QString("fieldy"), QString::number(fieldy));
		msg->addField(QString("fieldstate"), QString::number(state));

		if(m_connection->type() == KonnectionHandling::SERVER)
			m_kbserver->sendMessage(msg);
		else
			m_kbclient->sendMessage(msg);
	}
}

void KBattleshipApp::slotSendMessage(KMessage *msg)
{
	if(m_connection != 0)
	{
		if(m_connection->type() == KonnectionHandling::SERVER)
			m_kbserver->sendMessage(msg);
		else
			m_kbclient->sendMessage(msg);
	}
}

void KBattleshipApp::slotSendChatMessage(const QString &text)
{
	if(m_connection != 0 && m_serverHasClient)
	{
		KMessage *msg = new KMessage(KMessage::CHAT);
		msg->chatMessage(m_ownNickname, text);
		slotSendMessage(msg);
	}
}

void KBattleshipApp::slotChangedNickCommand(const QString &text)
{
	m_ownNickname = text;
	slotChangeOwnPlayer(m_ownNickname);
	m_chat->setNickname(m_ownNickname);
}

KShip *KBattleshipApp::shipAt(int fieldx, int fieldy)
{
	return m_ownshiplist->shipAt(fieldx, fieldy);
}

void KBattleshipApp::slotUpdateHighscore()
{
	m_score->save(m_ownNickname, m_stat->shot(), m_stat->hit(), m_stat->water());
	m_score->load();
}

void KBattleshipApp::saveOptions()
{
	m_config->setGroup("General");
	m_config->writeEntry("ShowStatusbar", m_viewStatusBar->isChecked());
	if(!m_sound->serverError())
		m_config->writeEntry("PlaySounds", m_configSound->isChecked());
	m_config->writeEntry("ShowGrid", m_configGrid->isChecked());
	m_config->sync();
}

void KBattleshipApp::readOptions()
{
	m_config->setGroup("General");

	m_viewStatusBar->setChecked(m_config->readBoolEntry("ShowStatusbar", true));
	slotViewStatusBar();

	m_configSound->setChecked(m_config->readBoolEntry("PlaySounds", true));
	m_configGrid->setChecked(m_config->readBoolEntry("ShowGrid", false));
}

void KBattleshipApp::slotGameQuit()
{
	delete this;
	kapp->quit();
}

void KBattleshipApp::slotHighscore()
{
	m_score->setCaption(i18n("Highscore"));
	m_score->load();
	m_score->show();
}

void KBattleshipApp::slotEnemyClientInfo()
{
	KInfoDialog *m_info = new KInfoDialog(this);

	m_info->lbl_clientIdentfier->setText(m_enemyClient);
	m_info->lbl_clientVersion->setText(m_enemyClientVersion);
	m_info->lbl_ClientInformation->setText(m_enemyClientDescription);
	m_info->lbl_ProtocolVersion->setText(m_enemyProtocolVersion);

	m_info->show();
}

void KBattleshipApp::slotServerConnect()
{
	if(m_connection == 0)
	{
		if(m_client != 0)
			return;

		slotStatusMsg(i18n("Loading Connect-Server dialog..."));

		m_client = new KClientDialog();
		connect(m_client, SIGNAL(sigConnectServer()), this, SLOT(slotConnectToBattleshipServer()));
		connect(m_client, SIGNAL(sigCancelConnect()), this, SLOT(slotDeleteConnectDialog()));
		m_client->show();

		slotStatusMsg(i18n("Ready"));
	}
	else
		slotAbortNetworkGame();
}

void KBattleshipApp::slotDeleteConnectDialog()
{
	delete m_client;
	m_client = 0;
}

void KBattleshipApp::slotDeleteClient()
{
	delete m_kbclient;
	m_kbclient = 0;
}

void KBattleshipApp::slotReplayRequest()
{
	switch(KMessageBox::questionYesNo(this, i18n("The client asks for restarting the game. Do you accept?")))
	{
		case KMessageBox::Yes:
			slotReplay();
			slotStatusMsg(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
			break;

		case KMessageBox::No:
			slotAbortNetworkGame();
			break;
	}
}

void KBattleshipApp::slotServerReplay()
{
	KMessage *msg = new KMessage(KMessage::REPLAY);
	switch(KMessageBox::questionYesNo(this, i18n("Do you want to restart the game?")))
	{
		case KMessageBox::Yes:
			slotReplay();
			slotStatusMsg(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
			slotSendMessage(msg);
			break;

		case KMessageBox::No:
			delete msg;
			slotAbortNetworkGame();
			break;
	}
}

void KBattleshipApp::slotClientReplay()
{
	KMessage *msg = new KMessage(KMessage::REPLAY);
	switch(KMessageBox::questionYesNo(this, i18n("Do you want to ask the server restarting the game?")))
	{
		case KMessageBox::Yes:
			slotReplay();
			slotStatusMsg(i18n("Waiting for an answer..."));
			slotSendMessage(msg);
			break;

		case KMessageBox::No:
			delete msg;
			slotAbortNetworkGame();
			break;
	}
}

void KBattleshipApp::cleanup(bool placechange)
{
	if(placechange)
		m_placeable = false;
	m_view->field()->setDrawField(false);
	m_ownshiplist->clear();
	m_enemyshiplist->clear();
	m_view->clearField();
	m_view->field()->setDrawField(true);
	m_view->field()->drawField();
}

void KBattleshipApp::slotNewServer()
{
	if(m_connection == 0)
	{
		if(m_server != 0)
			return;

		slotStatusMsg(i18n("Loading Start-Server dialog..."));

		m_server = new KServerDialog();
		connect(m_server, SIGNAL(sigStartServer()), this, SLOT(slotStartBattleshipServer()));
		connect(m_server, SIGNAL(sigCancelServer()), this, SLOT(slotDeleteServerDialog()));
		m_server->show();

		slotStatusMsg(i18n("Ready"));
	}
	else
		slotAbortNetworkGame();
}

void KBattleshipApp::slotDeleteServerDialog()
{
	delete m_server;
	m_server = 0;
}

void KBattleshipApp::slotSendVersion()
{
	KMessage *msg = new KMessage(KMessage::GETVERSION);
	msg->versionMessage();
	slotSendMessage(msg);

	QTimer::singleShot(150, this, SLOT(slotSendGreet()));
}

void KBattleshipApp::slotSendGreet()
{
	m_serverHasClient = true;
	m_chat->slotAcceptMsg(true);

	KMessage *msg = new KMessage(KMessage::GREET);
	msg->addField(QString("nickname"), m_ownNickname);
	slotSendMessage(msg);
}

void KBattleshipApp::slotStartBattleshipServer()
{
	m_gameNewServer->setText(i18n("&Stop server"));
	m_gameServerConnect->setEnabled(false);
	m_gameSingle->setEnabled(false);
	slotStatusMsg(i18n("Waiting for a player..."));
	m_kbserver = new KBattleshipServer((m_server->port()).toInt());
	m_ownNickname = m_server->nickname();
	m_chat->setNickname(m_ownNickname);
	slotChangeOwnPlayer(m_ownNickname);
	delete m_server;
	m_server = 0;
	cleanup(true);
	m_aiPlaying = false;
	m_shootable = false;
	m_placeable = true;
	m_stat->clear();
	m_stat->clearWon();
	if(m_connection == 0)
	{
		m_connection = new KonnectionHandling(this, m_kbserver);
		connect(m_connection, SIGNAL(sigStatusBar(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
		connect(m_connection, SIGNAL(sigEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
		connect(m_connection, SIGNAL(sigSendNickname()), this, SLOT(slotSendGreet()));
		connect(m_connection, SIGNAL(sigPlaceShips(bool)), this, SLOT(slotSetPlaceable(bool)));
		connect(m_connection, SIGNAL(sigShootable(bool)), this, SLOT(slotSetShootable(bool)));
		connect(m_connection, SIGNAL(sigSendFieldState(int, int)), this, SLOT(slotSendEnemyFieldState(int, int)));
		connect(m_connection, SIGNAL(sigEnemyFieldData(int, int, int, int, int, int, int, bool)), this, SLOT(slotReceivedEnemyFieldData(int, int, int, int, int, int, int, bool)));
		connect(m_connection, SIGNAL(sigClientLost()), this, SLOT(slotClientLost()));
		connect(m_connection, SIGNAL(sigAbortNetworkGame()), this, SLOT(slotAbortNetworkGame()));
		connect(m_connection, SIGNAL(sigReplay()), this, SLOT(slotReplayRequest()));
		connect(m_connection, SIGNAL(sigChatMessage(const QString &, const QString &, bool)), m_chat, SLOT(slotReceivedMessage(const QString &, const QString &, bool)));
		connect(m_connection, SIGNAL(sigClientInformation(const QString &, const QString &, const QString &, const QString &)), this, SLOT(slotReceivedClientInformation(const QString &, const QString &, const QString &, const QString &)));
		connect(m_connection, SIGNAL(sigLost()), this, SLOT(slotLost()));
	}
	else
	{
		if(m_connection->type() == KonnectionHandling::CLIENT)
		{
			disconnect(m_kbclient, SIGNAL(sigConnected()), this, SLOT(slotSendVersion()));
			disconnect(m_connection, SIGNAL(sigAbortNetworkGame()), this, SLOT(slotAbortNetworkGame()));
			disconnect(m_connection, SIGNAL(sigStatusBar(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
			disconnect(m_connection, SIGNAL(sigEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
			disconnect(m_connection, SIGNAL(sigSendFieldState(int, int)), this, SLOT(slotSendEnemyFieldState(int, int)));
			disconnect(m_connection, SIGNAL(sigEnemyFieldData(int, int, int, int, int, int, int, bool)), this, SLOT(slotReceivedEnemyFieldData(int, int, int, int, int, int, int, bool)));
			disconnect(m_connection, SIGNAL(sigShootable(bool)), this, SLOT(slotSetShootable(bool)));
			disconnect(m_connection, SIGNAL(sigPlaceShips(bool)), this, SLOT(slotSetPlaceable(bool)));
			disconnect(m_connection, SIGNAL(sigServerLost()), this, SLOT(slotServerLost()));
			disconnect(m_connection, SIGNAL(sigReplay()), this, SLOT(slotReplay()));
			disconnect(m_connection, SIGNAL(sigChatMessage(const QString &, const QString &, bool)), m_chat, SLOT(slotReceivedMessage(const QString &, const QString &, bool)));
			disconnect(m_connection, SIGNAL(sigLost()), this, SLOT(slotLost()));
			m_connection->updateInternal(m_kbserver);
			connect(m_connection, SIGNAL(sigStatusBar(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
			connect(m_connection, SIGNAL(sigEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
			connect(m_connection, SIGNAL(sigSendNickname()), this, SLOT(slotSendGreet()));
			connect(m_connection, SIGNAL(sigPlaceShips(bool)), this, SLOT(slotSetPlaceable(bool)));
			connect(m_connection, SIGNAL(sigShootable(bool)), this, SLOT(slotSetShootable(bool)));
			connect(m_connection, SIGNAL(sigSendFieldState(int, int)), this, SLOT(slotSendEnemyFieldState(int, int)));
			connect(m_connection, SIGNAL(sigEnemyFieldData(int, int, int, int, int, int, int, bool)), this, SLOT(slotReceivedEnemyFieldData(int, int, int, int, int, int, int, bool)));
			connect(m_connection, SIGNAL(sigClientLost()), this, SLOT(slotClientLost()));
			connect(m_connection, SIGNAL(sigAbortNetworkGame()), this, SLOT(slotAbortNetworkGame()));
			connect(m_connection, SIGNAL(sigReplay()), this, SLOT(slotReplayRequest()));
			connect(m_connection, SIGNAL(sigChatMessage(const QString &, const QString &, bool)), m_chat, SLOT(slotReceivedMessage(const QString &, const QString &, bool)));
			connect(m_connection, SIGNAL(sigLost()), this, SLOT(slotLost()));
		}
		else
			m_connection->updateInternal(m_kbserver);
	}
	m_kbserver->init();
}

void KBattleshipApp::slotLost()
{
	m_stat->slotAddEnemyWon();
	m_lost = true;
}

void KBattleshipApp::slotSendEnemyFieldState(int fieldx, int fieldy)
{
	int data, showstate;
	bool xokay = false, yokay = false;
	typedef QValueList<int> DeathValueList;
	DeathValueList deathList;

	data = m_ownshiplist->shipTypeAt(fieldx, fieldy);
	if(data == 99)
		showstate = KBattleField::WATER;
	else
		showstate = KBattleField::HIT;

	slotChangeOwnFieldData(fieldx, fieldy, showstate);

	KMessage *msg = new KMessage(KMessage::ANSWER_SHOOT);

	if(showstate == KBattleField::HIT)
	{
		if(m_ownshiplist->shipTypeAt(fieldx, fieldy) != 0 && m_ownshiplist->shipTypeAt(fieldx, fieldy) != 99)
		{
			KShip *ship = m_ownshiplist->shipAt(fieldx, fieldy);
			int tempy = 0, tempx = 0;

			if(ship->shipystart() == ship->shipystop() && ship->shipxstart() != ship->shipxstop())
			{
				for(tempx = ship->shipxstart(); tempx <= ship->shipxstop(); tempx++)
				{
					if(m_view->ownFieldState(tempx, fieldy) == KBattleField::HIT)
					{
						deathList.append(tempx);
						xokay = true;
						yokay = false;
					}
					else
					{
						xokay = false;
						yokay = false;
						break;
					}
				}
			}
			else if(ship->shipystart() != ship->shipystop() && ship->shipxstart() == ship->shipxstop())
			{
				for(tempy = ship->shipystart(); tempy <= ship->shipystop(); tempy++)
				{
					if(m_view->ownFieldState(fieldx, tempy) == KBattleField::HIT)
					{
						deathList.append(tempy);
						xokay = false;
						yokay = true;
					}
					else
					{
						xokay = false;
						yokay = false;
						break;
					}
				}
			}
		}
		else if(m_ownshiplist->shipTypeAt(fieldx, fieldy) == 0)
		{
			msg->addField(QString("xstart"), QString::number(fieldx));
			msg->addField(QString("xstop"), QString::number(fieldx));
			msg->addField(QString("ystart"), QString::number(fieldy));
			msg->addField(QString("ystop"), QString::number(fieldy));
			msg->addField(QString("death"), QString("true"));
		}
	}

	msg->addField(QString("fieldx"), QString::number(fieldx));
	msg->addField(QString("fieldy"), QString::number(fieldy));
	msg->addField(QString("fieldstate"), QString::number(data));

	if(xokay)
	{
		msg->addField(QString("xstart"), QString::number(deathList.first()));
		msg->addField(QString("xstop"), QString::number(deathList.last()));
		msg->addField(QString("ystart"), QString::number(fieldy));
		msg->addField(QString("ystop"), QString::number(fieldy));
		msg->addField(QString("death"), QString("true"));
	}
	else if(yokay)
	{
		msg->addField(QString("xstart"), QString::number(fieldx));
		msg->addField(QString("xstop"), QString::number(fieldx));
		msg->addField(QString("ystart"), QString::number(deathList.first()));
		msg->addField(QString("ystop"), QString::number(deathList.last()));
		msg->addField(QString("death"), QString("true"));
	}

	if(m_connection->type() == KonnectionHandling::SERVER)
		m_kbserver->sendMessage(msg);
	else
		m_kbclient->sendMessage(msg);
}

void KBattleshipApp::slotChangeOwnFieldData(int fieldx, int fieldy, int type)
{
	m_view->changeOwnFieldData(fieldx, fieldy, type);
	playSound(true, type);
}

void KBattleshipApp::playSound(bool enemy, int fieldstate)
{
	switch(fieldstate)
	{
		case KBattleField::WATER:
			m_sound->playSound(KBattleshipSound::PLAYER_SHOOT_WATER);
			break;

		case KBattleField::HIT:
			if(enemy)
				m_sound->playSound(KBattleshipSound::PLAYER1_SHOOT_HIT);
			else
				m_sound->playSound(KBattleshipSound::PLAYER2_SHOOT_HIT);
			break;

		case KBattleField::DEATH:
			m_sound->playSound(KBattleshipSound::SHIP_SINK);
			break;
	}
}

void KBattleshipApp::slotChangeEnemyFieldData(int fieldx, int fieldy, int type)
{
	m_view->changeEnemyFieldData(fieldx, fieldy, type);
	playSound(false, type);
}

void KBattleshipApp::slotConnectToBattleshipServer()
{
	m_kbclient = new KBattleshipClient(m_client->host(), (m_client->port()).toInt());
	m_ownNickname = m_client->nickname();
	m_chat->setNickname(m_ownNickname);
	slotChangeOwnPlayer(m_ownNickname);
	delete m_client;
	m_client = 0;
	cleanup(true);
	m_aiPlaying = false;
	m_shootable = false;
	m_placeable = false;
	m_stat->clear();
	m_stat->clearWon();
	m_gameServerConnect->setText(i18n("Dis&connect from server"));
	m_gameNewServer->setEnabled(false);
	m_gameSingle->setEnabled(false);
	if(m_connection == 0)
	{
		m_connection = new KonnectionHandling(this, m_kbclient);
		connect(m_kbclient, SIGNAL(sigConnected()), this, SLOT(slotSendVersion()));
		connect(m_connection, SIGNAL(sigAbortNetworkGame()), this, SLOT(slotAbortNetworkGame()));
		connect(m_connection, SIGNAL(sigStatusBar(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
		connect(m_connection, SIGNAL(sigEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
		connect(m_connection, SIGNAL(sigSendFieldState(int, int)), this, SLOT(slotSendEnemyFieldState(int, int)));
		connect(m_connection, SIGNAL(sigEnemyFieldData(int, int, int, int, int, int, int, bool)), this, SLOT(slotReceivedEnemyFieldData(int, int, int, int, int, int, int, bool)));
		connect(m_connection, SIGNAL(sigShootable(bool)), this, SLOT(slotSetShootable(bool)));
		connect(m_connection, SIGNAL(sigPlaceShips(bool)), this, SLOT(slotSetPlaceable(bool)));
		connect(m_connection, SIGNAL(sigServerLost()), this, SLOT(slotServerLost()));
		connect(m_connection, SIGNAL(sigReplay()), this, SLOT(slotReplay()));
		connect(m_connection, SIGNAL(sigChatMessage(const QString &, const QString &, bool)), m_chat, SLOT(slotReceivedMessage(const QString &, const QString &, bool)));
		connect(m_connection, SIGNAL(sigClientInformation(const QString &, const QString &, const QString &, const QString &)), this, SLOT(slotReceivedClientInformation(const QString &, const QString &, const QString &, const QString &)));
		connect(m_connection, SIGNAL(sigLost()), this, SLOT(slotLost()));
	}
	else
	{
		if(m_connection->type() == KonnectionHandling::SERVER)
		{
			disconnect(m_connection, SIGNAL(sigStatusBar(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
			disconnect(m_connection, SIGNAL(sigEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
			disconnect(m_connection, SIGNAL(sigSendNickname()), this, SLOT(slotSendGreet()));
			disconnect(m_connection, SIGNAL(sigPlaceShips(bool)), this, SLOT(slotSetPlaceable(bool)));
			disconnect(m_connection, SIGNAL(sigShootable(bool)), this, SLOT(slotSetShootable(bool)));
			disconnect(m_connection, SIGNAL(sigSendFieldState(int, int)), this, SLOT(slotSendEnemyFieldState(int, int)));
			disconnect(m_connection, SIGNAL(sigEnemyFieldData(int, int, int, int, int, int, int, bool)), this, SLOT(slotReceivedEnemyFieldData(int, int, int, int, int, int, int, bool)));
			disconnect(m_connection, SIGNAL(sigClientLost()), this, SLOT(slotClientLost()));
			disconnect(m_connection, SIGNAL(sigAbortNetworkGame()), this, SLOT(slotAbortNetworkGame()));
			disconnect(m_connection, SIGNAL(sigReplay()), this, SLOT(slotReplayRequest()));
			disconnect(m_connection, SIGNAL(sigChatMessage(const QString &, const QString &, bool)), m_chat, SLOT(slotReceivedMessage(const QString &, const QString &, bool)));
			disconnect(m_connection, SIGNAL(sigLost()), this, SLOT(slotLost()));
			m_connection->updateInternal(m_kbclient);
			connect(m_kbclient, SIGNAL(sigConnected()), this, SLOT(slotSendVersion()));
			connect(m_connection, SIGNAL(sigAbortNetworkGame()), this, SLOT(slotAbortNetworkGame()));
			connect(m_connection, SIGNAL(sigStatusBar(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
			connect(m_connection, SIGNAL(sigEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
			connect(m_connection, SIGNAL(sigSendFieldState(int, int)), this, SLOT(slotSendEnemyFieldState(int, int)));
			connect(m_connection, SIGNAL(sigEnemyFieldData(int, int, int, int, int, int, int, bool)), this, SLOT(slotReceivedEnemyFieldData(int, int, int, int, int, int, int, bool)));
			connect(m_connection, SIGNAL(sigShootable(bool)), this, SLOT(slotSetShootable(bool)));
			connect(m_connection, SIGNAL(sigPlaceShips(bool)), this, SLOT(slotSetPlaceable(bool)));
			connect(m_connection, SIGNAL(sigServerLost()), this, SLOT(slotServerLost()));
			connect(m_connection, SIGNAL(sigReplay()), this, SLOT(slotReplay()));
			connect(m_connection, SIGNAL(sigChatMessage(const QString &, const QString &, bool)), m_chat, SLOT(slotReceivedMessage(const QString &, const QString &, bool)));
			m_kbclient->init();
			connect(m_connection, SIGNAL(sigClientInformation(const QString &, const QString &, const QString &, const QString &)), this, SLOT(slotReceivedClientInformation(const QString &, const QString &, const QString &, const QString &)));
			connect(m_connection, SIGNAL(sigLost()), this, SLOT(slotLost()));
		}
		else
			m_connection->updateInternal(m_kbclient);
	}
	m_kbclient->init();
}

void KBattleshipApp::slotSetPlaceable(bool place)
{
	m_placeable = place;
}

void KBattleshipApp::slotSetShootable(bool shoot)
{
	m_shootable = shoot;
}

void KBattleshipApp::slotShowGrid()
{
	if(!m_configGrid->isChecked())
		m_view->field()->disableGrid();
	else
		m_view->field()->enableGrid();
}

void KBattleshipApp::slotConfigSound()
{
	if(!m_configSound->isChecked())
		m_sound->turnOff();
	else
		m_sound->turnOn();
}

void KBattleshipApp::slotViewStatusBar()
{
	if(!m_viewStatusBar->isChecked())
		statusBar()->hide();
	else
		statusBar()->show();
}

void KBattleshipApp::slotStatusMsg(const QString &text)
{
	statusBar()->clear();
	statusBar()->changeItem(text, ID_STATUS_MSG);
}

void KBattleshipApp::slotChangeOwnPlayer(const QString &text)
{
	statusBar()->clear();
	statusBar()->changeItem(i18n("     Player 1: %1     ").arg(text), ID_PLAYER_OWN);
}

void KBattleshipApp::slotChangeEnemyPlayer(const QString &text)
{
	statusBar()->clear();
	statusBar()->changeItem(i18n("     Player 2: %1     ").arg(text), ID_PLAYER_ENEMY);
}

void KBattleshipApp::slotSinglePlayer()
{
	if(!m_aiPlaying)
	{
		if(m_single != 0)
			return;

		slotStatusMsg(i18n("Loading Single-Game dialog..."));

		m_single = new KSingleDialog();
		connect(m_single, SIGNAL(sigStartGame()), this, SLOT(slotStartBattleshipGame()));
		connect(m_single, SIGNAL(sigCancelGame()), this, SLOT(slotDeleteSingleDialog()));
		m_single->show();

		slotStatusMsg(i18n("Ready"));
	}
	else
	{
		if(m_aiPlayer != 0)
		{
			m_aiPlaying = false;
			slotChangeOwnPlayer("-");
			slotChangeEnemyPlayer("-");
			m_gameSingle->setText(i18n("S&ingle Player"));
			m_gameNewServer->setEnabled(true);
			m_gameServerConnect->setEnabled(true);
			slotStatusMsg(i18n("Ready"));
			m_stat->clear();
			m_chat->clear();
			QTimer::singleShot(0, this, SLOT(slotDeleteAI()));
			cleanup(false);
		}
	}
}

void KBattleshipApp::slotDeleteSingleDialog()
{
	delete m_single;
	m_single = 0;
}


void KBattleshipApp::slotStartBattleshipGame()
{
	slotStartBattleshipGame(true);
}

void KBattleshipApp::slotStartBattleshipGame(bool clearstat)
{
	m_gameSingle->setText(i18n("&Stop game"));
	m_gameNewServer->setEnabled(false);
	m_gameServerConnect->setEnabled(false);
	slotStatusMsg(i18n("Waiting for the AI player to place the ships..."));
	if(m_single != 0)
	{
		m_ownNickname = m_single->nickname();
		delete m_single;
		m_single = 0;
	}
	slotChangeOwnPlayer(m_ownNickname);
	slotChangeEnemyPlayer(KGameMisc::randomName());
	cleanup(true);
	if(m_connection != 0)
	{
		delete m_connection;
		m_connection = 0;
	}
	m_aiPlaying = true;
	m_shootable = false;
	m_stat->clear();
	if(clearstat)
		m_stat->clearWon();

	if(m_aiPlayer == 0)
	{
		m_aiPlayer = new KBAIPlayer();
		m_aiPlayer->init(m_view->field(), m_enemyshiplist);
		connect(m_aiPlayer, SIGNAL(sigReady()), this, SLOT(slotAIReady()));
		connect(m_aiPlayer, SIGNAL(sigShootAt(const QPoint)), this, SLOT(slotAIShootsAt(const QPoint)));
	}
	m_aiPlayer->slotRestart();
}

void KBattleshipApp::slotAIReady()
{
	slotStatusMsg(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
	m_placeable = true;
}

void KBattleshipApp::slotAIShootsAt(const QPoint pos)
{
	if(!m_shootable)
		m_shootable = true;

	int showstate = m_view->ownFieldState(pos.x(), pos.y());

	if(showstate == KBattleField::HIT)
	{
		m_aiPlayer->slotRequestShot();
		return;
	}
	else if(showstate == KBattleField::FREE)
		showstate = KBattleField::WATER;
	else if(showstate >= KBattleField::SHIP1P1)
	{
		showstate = KBattleField::HIT;
		m_aiHits++;
	}

	slotStatusMsg(i18n("Enemy has shot. Shoot now"));
	slotChangeOwnFieldData(pos.x(), pos.y(), showstate);

	if(m_aiHits == 10 && m_stat->hit() != 10)
	{
		m_aiPlaying = false;
		m_shootable = false;
		slotChangeOwnPlayer("-");
		slotChangeEnemyPlayer("-");
		m_gameSingle->setText(i18n("S&ingle Player"));
		m_gameNewServer->setEnabled(true);
		m_gameServerConnect->setEnabled(true);
		slotStatusMsg(i18n("You lost the game :("));
		m_stat->slotAddEnemyWon();
		slotUpdateHighscore();
		switch(KMessageBox::questionYesNo(this, i18n("Do you want to restart the game?")))
		{
			case KMessageBox::Yes:
				QTimer::singleShot(0, this, SLOT(slotRestartAI()));
				break;

			case KMessageBox::No:
				QTimer::singleShot(0, this, SLOT(slotDeleteAI()));
				break;
		}
	}
	else
	{
		if(!m_shootable)
			m_shootable = true;
	}
}

void KBattleshipApp::slotReceivedClientInformation(const QString &clientName, const QString &clientVersion, const QString &clientDescription, const QString &protocolVersion)
{
	m_enemyClient = clientName;
	m_enemyClientVersion = clientVersion;
	m_enemyClientDescription = clientDescription;
	m_enemyProtocolVersion = protocolVersion;
	m_gameEnemyInfo->setEnabled(true);

	if(m_connection->type() == KonnectionHandling::SERVER)
	{
		KMessage *msg = new KMessage(KMessage::GETVERSION);
		msg->versionMessage();
		slotSendMessage(msg);
	}
}
