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

#include <qlayout.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QGridLayout>

#include <kgamemisc.h>
#include <kinputdialog.h>
#include <knotifyclient.h>
#include <knotifydialog.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>
#include <kstdgameaction.h>
#include <kcmdlineargs.h>
#include <kmessagebox.h>
#include <kuser.h>

#include <kscoredialog.h>
#include <kglobal.h>


#include "kbattleship.moc"

extern const char *clientVersion;

KBattleshipWindow::KBattleshipWindow() : KMainWindow()
{
	shift = false;
	m_connection = 0;
	m_lost = 0;
	m_config = 0;
	m_client = 0;
	m_server = 0;
	m_aiPlayer = 0;
	m_aiHits = 0;

	init();
}

KBattleshipWindow::~KBattleshipWindow()
{
	if(m_config != 0)
		saveOptions();
	delete m_aiPlayer;
	delete m_ownshiplist;
	delete m_enemyshiplist;
}

void KBattleshipWindow::init()
{
	m_aiPlaying = false;
	m_placeable = false;
	m_shootable = false;
	m_serverHasClient = false;
	m_config = KGlobal::config();
	initStatusBar();
	initActions();
	readOptions();
	initView();
	initChat();
	initShipPlacing();
	parseCommandLine();
}

void KBattleshipWindow::slotConfigureNotifications()
{
  KNotifyDialog::configure(this);
}

void KBattleshipWindow::initStatusBar()
{
	m_ownNickname = "-";
	m_enemyNickname = "-";
	statusBar()->insertPermanentItem(i18n("     Player 1: %1     ").arg(m_ownNickname), ID_PLAYER_OWN, 0);
	statusBar()->insertPermanentItem(i18n("     Player 2: %1     ").arg(m_enemyNickname), ID_PLAYER_ENEMY, 0);
	statusBar()->insertItem(i18n("Ready"), ID_STATUS_MSG, 1);
	statusBar()->setItemAlignment(ID_STATUS_MSG, Qt::AlignLeft);
}

void KBattleshipWindow::initActions()
{
	KStdAction::configureNotifications(this, SLOT(slotConfigureNotifications()), actionCollection());
	m_gameServerConnect = new KAction(i18n("&Connect to Server..."), "connect_no", Qt::Key_F2, this, SLOT(slotServerConnect()), actionCollection(), "game_serverconnect");
	m_gameNewServer = new KAction(i18n("&Start Server..."), "network", Qt::Key_F3, this, SLOT(slotNewServer()), 
actionCollection(), "game_newserver");
	m_gameSingle = new KAction(i18n("S&ingle Player..."), "gear", Qt::Key_F4, this, SLOT(slotSinglePlayer()), actionCollection(), "game_singleplayer");
	m_gameQuit = KStdGameAction::quit(this, SLOT(close()), actionCollection());
	KStdGameAction::highscores(this, SLOT(slotHighscore()), actionCollection());
	m_gameEnemyInfo = new KAction(i18n("&Enemy Info"), "view_text", Qt::Key_F11, this, SLOT(slotEnemyClientInfo()), actionCollection(), "game_enemyinfo");

	m_configSound = new KToggleAction(i18n("&Play Sounds"), 0, actionCollection(), "options_configure_sound");
	m_configGrid = new KToggleAction(i18n("&Show Grid"), 0, this, SLOT(slotShowGrid()), actionCollection(), "options_show_grid");
	m_configGrid->setCheckedState(i18n("Hide Grid"));

	m_gameEnemyInfo->setEnabled(false);

	setupGUI( KMainWindow::Save | StatusBar | Keys | Create );
}

void KBattleshipWindow::initChat()
{
	connect(m_chat, SIGNAL(sigSendMessage(const QString &)), this, SLOT(slotSendChatMessage(const QString &)));
	connect(m_chat, SIGNAL(sigChangeEnemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
	connect(m_chat, SIGNAL(sigChangeOwnNickname(const QString &)), this, SLOT(slotChangedNickCommand(const QString &)));
}

void KBattleshipWindow::changeShipPlacementDirection(){
  shift = !shift;
}

void KBattleshipWindow::initShipPlacing()
{
	connect(m_ownshiplist, SIGNAL(sigOwnFieldDataChanged(int, int, int)), this, SLOT(slotChangeOwnFieldData(int, int, int)));
	connect(m_ownshiplist, SIGNAL(sigLastShipAdded()), this, SLOT(slotShipsReady()));
}

void KBattleshipWindow::initView()
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

	m_view->startDrawing();
	setFocusProxy(m_view);

	connect(m_view, SIGNAL(sigEnemyFieldClicked(int, int)), this, SLOT(slotEnemyFieldClick(int, int)));
	connect(m_view, SIGNAL(sigOwnFieldClicked(int, int)), this, SLOT(slotPlaceShip(int, int)));
	connect(m_view, SIGNAL(sigMouseOverField(int, int)), this, SLOT(slotPlaceShipPreview(int, int)));
	connect(m_view, SIGNAL(changeShipPlacementDirection()), this, SLOT(changeShipPlacementDirection()));
}

void KBattleshipWindow::slotDeleteAI()
{
	m_aiHits = 0;
	delete m_aiPlayer;
	m_aiPlayer = 0;
}

void KBattleshipWindow::slotRestartAI()
{
	m_aiHits = 0;
	slotStartBattleshipGame(false);
}

void KBattleshipWindow::slotEnemyFieldClick(int fieldx, int fieldy)
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
					KShip *ship = m_enemyshiplist->shipAt(fieldx, fieldy);
					typedef QList<int> DeathValueList;
					DeathValueList deathList;
					bool xokay = true, yokay = true;
					int tempy = 0, tempx = 0;

					if(ship->placedLeft())
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
					else
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
						for(it = deathList.begin(); it != deathList.end(); ++it)
						{
							if(fieldy+1 < m_enemyshiplist->m_fieldy) m_view->changeEnemyFieldData(*it, fieldy+1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(*it, fieldy, KBattleField::DEATH);
							if(fieldy > 0) m_view->changeEnemyFieldData(*it, fieldy-1, KBattleField::BORDER);
						}
						if(ship->shipxstart() > 0)
						{
							if (fieldy > 0) m_view->changeEnemyFieldData(ship->shipxstart()-1, fieldy-1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(ship->shipxstart()-1, fieldy, KBattleField::BORDER);
							if (fieldy < m_enemyshiplist->m_fieldy) m_view->changeEnemyFieldData(ship->shipxstart()-1, fieldy+1, KBattleField::BORDER);
						}
						if(ship->shipxstop() < m_enemyshiplist->m_fieldx)
						{
							if (fieldy>0) m_view->changeEnemyFieldData(ship->shipxstop()+1, fieldy-1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(ship->shipxstop()+1, fieldy, KBattleField::BORDER);
							if (fieldy < m_enemyshiplist->m_fieldy)m_view->changeEnemyFieldData(ship->shipxstop()+1,fieldy+1, KBattleField::BORDER);
						}
					}
					else if(yokay)
					{
						DeathValueList::Iterator it;
						for(it = deathList.begin(); it != deathList.end(); ++it)
						{
							if (fieldx>0) m_view->changeEnemyFieldData(fieldx-1, *it, KBattleField::BORDER);
							m_view->changeEnemyFieldData(fieldx, *it, KBattleField::DEATH);
							if(fieldx<m_enemyshiplist->m_fieldx) m_view->changeEnemyFieldData(fieldx+1, *it, KBattleField::BORDER);
						}
						if(ship->shipystart()>0)
						{
							if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, ship->shipystart()-1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(fieldx, ship->shipystart()-1, KBattleField::BORDER);
							if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, ship->shipystart()-1, KBattleField::BORDER);
						}
						if(ship->shipystop()<m_enemyshiplist->m_fieldy)
						{
							if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, ship->shipystop()+1, KBattleField::BORDER);
							m_view->changeEnemyFieldData(fieldx, ship->shipystop()+1, KBattleField::BORDER);
							if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, ship->shipystop()+1, KBattleField::BORDER);
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
				m_gameSingle->setText(i18n("S&ingle Player..."));
				m_gameNewServer->setEnabled(true);
				m_gameServerConnect->setEnabled(true);
				slotStatusMsg(i18n("You won the game :)"));
				m_stat->slotAddOwnWon();
				slotUpdateHighscore();
				switch(KMessageBox::questionYesNo(this, i18n("Do you want to restart the game?"),QString::null,i18n("Restart"),i18n("Do Not Restart")))
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
			else if(m_aiPlayer != 0 && m_aiPlaying)
				m_aiPlayer->slotRequestShot();
		}
	}
}

void KBattleshipWindow::slotReceivedEnemyFieldData(int fieldx, int fieldy, int enemystate, int xstart, int xstop, int ystart, int ystop, bool death)
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
			{
				if (fieldx>0) m_view->changeEnemyFieldData(fieldx-1, i, KBattleField::BORDER);
				m_view->changeEnemyFieldData(fieldx, i, KBattleField::DEATH);
				if(fieldx<m_enemyshiplist->m_fieldx) m_view->changeEnemyFieldData(fieldx+1, i, KBattleField::BORDER);
			}
			if(ystart>0)
			{
				if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, ystart-1, KBattleField::BORDER);
				m_view->changeEnemyFieldData(fieldx, ystart-1, KBattleField::BORDER);
				if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, ystart-1, KBattleField::BORDER);
			}
			if(ystop<m_enemyshiplist->m_fieldy)
			{
				if (fieldx>0)m_view->changeEnemyFieldData(fieldx-1, ystop+1, KBattleField::BORDER);
				m_view->changeEnemyFieldData(fieldx, ystop+1, KBattleField::BORDER);
				if (fieldx<m_enemyshiplist->m_fieldx)m_view->changeEnemyFieldData(fieldx+1, ystop+1, KBattleField::BORDER);
			}
		}
		else if(ystart == ystop)
		{
			for(int i = xstart; i <= xstop; i++)
			{
				if(fieldy+1 < m_enemyshiplist->m_fieldy) m_view->changeEnemyFieldData(i, fieldy+1, KBattleField::BORDER);
				m_view->changeEnemyFieldData(i, fieldy, KBattleField::DEATH);
				if(fieldy > 0) m_view->changeEnemyFieldData(i, fieldy-1, KBattleField::BORDER);
			}
			if(xstart > 0)
			{
				if (fieldy > 0) m_view->changeEnemyFieldData(xstart-1, fieldy-1, KBattleField::BORDER);
				m_view->changeEnemyFieldData(xstart-1, fieldy, KBattleField::BORDER);
				if (fieldy < m_enemyshiplist->m_fieldy) m_view->changeEnemyFieldData(xstart-1, fieldy+1, KBattleField::BORDER);
			}
			if(xstop < m_enemyshiplist->m_fieldx)
			{
				if (fieldy>0) m_view->changeEnemyFieldData(xstop+1, fieldy-1, KBattleField::BORDER);
				m_view->changeEnemyFieldData(xstop+1, fieldy, KBattleField::BORDER);
				if (fieldy < m_enemyshiplist->m_fieldy)m_view->changeEnemyFieldData(xstop+1,fieldy+1, KBattleField::BORDER);
			}
		}
	}

	if(m_stat->hit() != 10)
		slotStatusMsg(i18n("Waiting for enemy to shoot.."));
	else
	{
		KMessage *msg = m_view->getAliveShips(m_ownshiplist); // let's show ai player ships
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

void KBattleshipWindow::slotClientLost()
{
	slotAbortNetworkGame();
	slotStatusMsg(i18n("Enemy disconnected."));
}

void KBattleshipWindow::slotServerLost()
{
	slotAbortNetworkGame();
	slotStatusMsg(i18n("Enemy disconnected."));
}

void KBattleshipWindow::slotAbortNetworkGame()
{
	slotStatusMsg(i18n("Ready"));
	slotChangeOwnPlayer("-");
	slotChangeEnemyPlayer("-");

	m_gameServerConnect->setText(i18n("&Connect to Server..."));
	m_gameNewServer->setText(i18n("&Start Server..."));
	m_gameSingle->setText(i18n("S&ingle Player..."));
	m_gameServerConnect->setEnabled(true);
	m_gameNewServer->setEnabled(true);
	m_gameSingle->setEnabled(true);
	m_gameEnemyInfo->setEnabled(false);

	m_chat->clear();

	m_aiPlaying = false;
	m_shootable = false;
	m_placeable = false;
	m_serverHasClient = false;

	if (m_connection)
	{
		if(m_connection->type() == KonnectionHandling::SERVER)
		{
			delete m_kbserver;
			m_kbserver = 0;
		}
		else
		{
			delete m_kbclient;
			m_kbclient = 0;
		}
		delete m_connection;
		m_connection = 0;
	}
}

void KBattleshipWindow::slotReplay()
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

void KBattleshipWindow::slotPlaceShipPreview(int fieldx, int fieldy)
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

			m_view->update();
		}
	}
}

void KBattleshipWindow::slotPlaceShip(int fieldx, int fieldy)
{
	if(m_connection != 0 || m_aiPlaying)
	{
		if(!m_aiPlaying && m_connection == 0)
			return;

		if(m_connection != 0 && !m_aiPlaying && !m_serverHasClient)
			return;

		if(m_placeable && m_ownshiplist->canAddShips())
			m_ownshiplist->addNewShip(shift, fieldx, fieldy);
	}
}

void KBattleshipWindow::slotShipsReady()
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
		slotStatusMsg(i18n("Waiting for other player to place their ships..."));
	else
		slotStatusMsg(i18n("Waiting for other player to start the match..."));

	m_placeable = false;
}

void KBattleshipWindow::slotSendMessage(int fieldx, int fieldy, int state)
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

void KBattleshipWindow::slotSendMessage(KMessage *msg)
{
	if(m_connection != 0)
	{
		if(m_connection->type() == KonnectionHandling::SERVER)
			m_kbserver->sendMessage(msg);
		else
			m_kbclient->sendMessage(msg);
	}
}

void KBattleshipWindow::slotSendChatMessage(const QString &text)
{
	if(m_connection != 0 && m_serverHasClient)
	{
		KMessage *msg = new KMessage(KMessage::CHAT);
		msg->chatMessage(m_ownNickname, text);
		slotSendMessage(msg);
	}
}

void KBattleshipWindow::slotChangedNickCommand(const QString &text)
{
	m_ownNickname = text;
	slotChangeOwnPlayer(m_ownNickname);
	m_chat->setNickname(m_ownNickname);
}

KShip *KBattleshipWindow::shipAt(int fieldx, int fieldy)
{
	return m_ownshiplist->shipAt(fieldx, fieldy);
}

KShip *KBattleshipWindow::enemyShipAt(int fieldx, int fieldy)
{
	return m_enemyshiplist->shipAt(fieldx, fieldy);
}

void KBattleshipWindow::slotUpdateHighscore()
{
	// Balancing factors
	// a = shot-balance
	// b = water-balance
	double a = 3;
	double b = 0.5;
	double score = (a * m_stat->hit() - b * m_stat->water()) / (m_stat->shot() + m_stat->water()) * 1000;
	if(score == 0) score = 1;
	
	KScoreDialog *scoreDialog = new KScoreDialog(KScoreDialog::Name | KScoreDialog::Score | KScoreDialog::Custom1 | KScoreDialog::Custom2 | KScoreDialog::Custom3, this);
	scoreDialog->addField(KScoreDialog::Custom1, i18n("Shots"), "shots");
	scoreDialog->addField(KScoreDialog::Custom2, i18n("Hits"), "hits");
	scoreDialog->addField(KScoreDialog::Custom3, i18n("Water"), "water");

	KScoreDialog::FieldInfo info;
	info[KScoreDialog::Name] = m_ownNickname;
	info[KScoreDialog::Custom1] = QString::number(m_stat->shot());
	info[KScoreDialog::Custom2] = QString::number(m_stat->hit());
	info[KScoreDialog::Custom3] = QString::number(m_stat->water());

	scoreDialog->addScore((int)score, info, false, false);
}

void KBattleshipWindow::saveOptions()
{
	m_config->setGroup("General");
	m_config->writeEntry("PlaySounds", m_configSound->isChecked());
	m_config->writeEntry("ShowGrid", m_configGrid->isChecked());
	m_config->sync();
}

void KBattleshipWindow::readOptions()
{
	m_config->setGroup("General");
	m_configSound->setChecked(m_config->readEntry("PlaySounds", true));
	m_configGrid->setChecked(m_config->readEntry("ShowGrid", false));
}

void KBattleshipWindow::slotHighscore()
{
	KScoreDialog *scoreDialog = new KScoreDialog(KScoreDialog::Name | KScoreDialog::Score | KScoreDialog::Custom1 | KScoreDialog::Custom2 | KScoreDialog::Custom3, this);
	scoreDialog->addField(KScoreDialog::Custom1, i18n("Shots"), "shots");
	scoreDialog->addField(KScoreDialog::Custom2, i18n("Hits"), "hits");
	scoreDialog->addField(KScoreDialog::Custom3, i18n("Water"), "water");
	scoreDialog->show();
}

void KBattleshipWindow::slotEnemyClientInfo()
{
	KInfoDialog *m_info = new KInfoDialog(this);

	m_info->lbl_clientIdentfier->setText(m_enemyClient);
	m_info->lbl_clientVersion->setText(m_enemyClientVersion);
	m_info->lbl_ClientInformation->setText(m_enemyClientDescription);
	m_info->lbl_ProtocolVersion->setText(m_enemyProtocolVersion);

	m_info->show();
}

void KBattleshipWindow::slotServerConnect()
{
	if(m_connection == 0)
	{
		if(m_client != 0) {
			m_client->show();
			return;
		}

		slotStatusMsg(i18n("Loading Connect-Server dialog..."));

		m_client = new KClientDialog(this);
		connect(m_client, SIGNAL(sigConnectServer()), this, SLOT(slotConnectToBattleshipServer()));
		connect(m_client, SIGNAL(sigCancelConnect()), this, SLOT(slotDeleteConnectDialog()));
		m_client->show();

		slotStatusMsg(i18n("Ready"));
	}
	else
		slotAbortNetworkGame();
}

void KBattleshipWindow::slotDeleteConnectDialog()
{
	delete m_client;
	m_client = 0;
}

void KBattleshipWindow::slotReplayRequest()
{
	switch(KMessageBox::questionYesNo(this, i18n("The client is asking to restart the game. Do you accept?"),QString::null,i18n("Accept Restart"), i18n("Deny Restart")))
	{
		case KMessageBox::Yes:
			if (m_connection)
			{ // the client could have closed while the user was thinking if he wanted to replay
				slotReplay();
				slotStatusMsg(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
			}
			else slotAbortNetworkGame();
			break;

		case KMessageBox::No:
			slotAbortNetworkGame();
			break;
	}
}

void KBattleshipWindow::slotServerReplay()
{
	KMessage *msg = new KMessage(KMessage::REPLAY);
	switch(KMessageBox::questionYesNo(this, i18n("Do you want to restart the game?"), QString::null, i18n("Restart"), i18n("Do Not Restart")))
	{
		case KMessageBox::Yes:
			if (m_connection)
			{ // the client could have closed while the user was thinking if he wanted to replay
				slotReplay();
				slotStatusMsg(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
				slotSendMessage(msg);
			}
			else
			{
				delete msg;
				slotAbortNetworkGame();
			}
			break;

		case KMessageBox::No:
			delete msg;
			slotAbortNetworkGame();
			break;
	}
}

void KBattleshipWindow::slotClientReplay()
{
	KMessage *msg = new KMessage(KMessage::REPLAY);
	switch(KMessageBox::questionYesNo(this, i18n("Do you want to ask the server restarting the game?"), QString::null, i18n("Ask to Restart"), i18n("Do Not Ask")))
	{
		case KMessageBox::Yes:
			if (m_connection)
			{ // the server could have closed while the user was thinking if he wanted to replay
				slotReplay();
				slotStatusMsg(i18n("Waiting for an answer..."));
				slotSendMessage(msg);
			}
			else
			{
				delete msg;
				slotAbortNetworkGame();
			}
			break;

		case KMessageBox::No:
			delete msg;
			slotAbortNetworkGame();
			break;
	}
}

void KBattleshipWindow::cleanup(bool placechange)
{
	if(placechange)
		m_placeable = false;
	m_view->field()->setDrawField(false);
	m_ownshiplist->clear();
	m_enemyshiplist->clear();
	m_view->clearField();
	m_view->field()->setDrawField(true);
	m_view->update();
}

void KBattleshipWindow::slotNewServer()
{
	if(m_connection == 0)
	{
		if(m_server != 0)
			return;

		slotStatusMsg(i18n("Loading Start-Server dialog..."));

		m_server = new KServerDialog(this);
		connect(m_server, SIGNAL(okClicked()), this, SLOT(slotStartBattleshipServer()));
		connect(m_server, SIGNAL(cancelClicked()), this, SLOT(slotDeleteServerDialog()));
		m_server->show();

		slotStatusMsg(i18n("Ready"));
	}
	else
		slotAbortNetworkGame();
}

void KBattleshipWindow::slotDeleteServerDialog()
{
	delete m_server;
	m_server = 0;
}

void KBattleshipWindow::slotSendVersion()
{
	KMessage *msg = new KMessage(KMessage::GETVERSION);
	msg->versionMessage();
	slotSendMessage(msg);

	QTimer::singleShot(150, this, SLOT(slotSendGreet()));
}

void KBattleshipWindow::slotSendGreet()
{
	m_serverHasClient = true;
	m_chat->slotAcceptMsg(true);

	KMessage *msg = new KMessage(KMessage::GREET);
	msg->addField(QString("nickname"), m_ownNickname);
	slotSendMessage(msg);
}

void KBattleshipWindow::slotStartBattleshipServer()
{
	m_gameNewServer->setText(i18n("&Stop server"));
	m_gameServerConnect->setEnabled(false);
	m_gameSingle->setEnabled(false);
	slotStatusMsg(i18n("Waiting for a player..."));
	m_kbserver = new KBattleshipServer((m_server->port()).toInt(),m_server->gamename());
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
		connect(m_connection, SIGNAL(sigLost(KMessage *)), this, SLOT(slotLost(KMessage *)));
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
			disconnect(m_connection, SIGNAL(sigLost(KMessage *)), this, SLOT(slotLost(KMessage *)));
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
			connect(m_connection, SIGNAL(sigLost(KMessage *)), this, SLOT(slotLost(KMessage *)));
		}
		else
			m_connection->updateInternal(m_kbserver);
	}
	m_kbserver->init();
}

void KBattleshipWindow::slotLost(KMessage *msg)
{
	m_stat->slotAddEnemyWon();
	if (!msg->field("ship0").isNull()) m_view->drawEnemyShipsHuman(msg, m_enemyshiplist);
	m_lost = true;
}

void KBattleshipWindow::slotSendEnemyFieldState(int fieldx, int fieldy)
{
	int data, showstate;
	bool xokay = false, yokay = false;
	typedef QList<int> DeathValueList;
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

void KBattleshipWindow::slotChangeOwnFieldData(int fieldx, int fieldy, int type)
{
	m_view->changeOwnFieldData(fieldx, fieldy, type);
	playSound(true, type);
}

void KBattleshipWindow::playSound(bool enemy, int fieldstate)
{
	if (m_configSound->isChecked())
	{
		switch(fieldstate)
		{
			case KBattleField::WATER:
				KNotifyClient::event(winId(), "shoot_water");
				break;

			case KBattleField::HIT:
				if(enemy)
					KNotifyClient::event(winId(), "shoot_hit_1");
				else
					KNotifyClient::event(winId(), "shoot_hit_2");
				break;

			case KBattleField::DEATH:
				KNotifyClient::event(winId(), "shoot_sink");
				break;
		}
	}
}

void KBattleshipWindow::slotChangeEnemyFieldData(int fieldx, int fieldy, int type)
{
	m_view->changeEnemyFieldData(fieldx, fieldy, type);
	playSound(false, type);
}

void KBattleshipWindow::parseCommandLine() {
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if ( args->count() > 0 )
	{
		KUrl u( args->url(0));
		if(u.protocol().isEmpty())
			u.setProtocol("kbattleship");
		if( !u.isValid()) {
			KMessageBox::sorry(this, 
			     i18n("The URL passed to KDE Battleship '%1' is not a valid url")
			         .arg(args->arg(0)));
			return;
		}
		if( u.protocol() != "kbattleship" ) {
			KMessageBox::sorry(this, 
			      i18n("The URL passed to KDE Battleship '%1' is not recognised "
			           "as a Battleship game.")
			          .arg(args->arg(0)));
			return;
		}

		slotConnectToBattleshipServer(u.host(), u.port(), u.user());
		
	}
		    
}

void KBattleshipWindow::slotConnectToBattleshipServer()
{
	QString host = m_client->host();
	int port = m_client->port().toInt();
	QString nickname = m_client->nickname();
	delete m_client;
	m_client = 0;
	slotConnectToBattleshipServer(host, port, nickname);
}
void KBattleshipWindow::slotConnectToBattleshipServer(const QString &host, int port, const QString &nickname)
{
	m_kbclient = new KBattleshipClient(host, port);
	nickname.isEmpty() ? m_ownNickname = "TestUser" : m_ownNickname = nickname;
	m_chat->setNickname(m_ownNickname);
	slotChangeOwnPlayer(m_ownNickname);
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
		connect(m_connection, SIGNAL(sigLost(KMessage *)), this, SLOT(slotLost(KMessage *)));
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
			disconnect(m_connection, SIGNAL(sigLost(KMessage *)), this, SLOT(slotLost(KMessage *)));
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
			connect(m_connection, SIGNAL(sigLost(KMessage *)), this, SLOT(slotLost(KMessage *)));
		}
		else
			m_connection->updateInternal(m_kbclient);
	}
	m_kbclient->init();
}

void KBattleshipWindow::slotSetPlaceable(bool place)
{
	m_placeable = place;
}

void KBattleshipWindow::slotSetShootable(bool shoot)
{
	m_shootable = shoot;
}

void KBattleshipWindow::slotShowGrid()
{
	if(!m_configGrid->isChecked())
		m_view->field()->disableGrid();
	else
		m_view->field()->enableGrid();
}

void KBattleshipWindow::slotStatusMsg(const QString &text)
{
	statusBar()->clear();
	statusBar()->changeItem(text, ID_STATUS_MSG);
}

void KBattleshipWindow::slotChangeOwnPlayer(const QString &text)
{
	statusBar()->clear();
	statusBar()->changeItem(i18n("     Player 1: %1     ").arg(text), ID_PLAYER_OWN);
}

void KBattleshipWindow::slotChangeEnemyPlayer(const QString &text)
{
	statusBar()->clear();
	statusBar()->changeItem(i18n("     Player 2: %1     ").arg(text), ID_PLAYER_ENEMY);
}

void KBattleshipWindow::slotSinglePlayer()
{
	bool ok;
	if(!m_aiPlaying)
	{
		KUser u;
		m_ownNickname = KInputDialog::getText(i18n("Start Game"), i18n("Nick name:"),
			u.loginName(), &ok, this);
		if (ok)
		{
			slotStatusMsg(i18n("Ready"));
			slotStartBattleshipGame();
		}
	}
	else
	{
		if(m_aiPlayer != 0)
		{
			m_aiPlaying = false;
			slotChangeOwnPlayer("-");
			slotChangeEnemyPlayer("-");
			m_gameSingle->setText(i18n("S&ingle Player..."));
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

void KBattleshipWindow::slotStartBattleshipGame()
{
	slotStartBattleshipGame(true);
}

void KBattleshipWindow::slotStartBattleshipGame(bool clearstat)
{
	m_gameSingle->setText(i18n("&Stop Game"));
	m_gameNewServer->setEnabled(false);
	m_gameServerConnect->setEnabled(false);
	slotStatusMsg(i18n("Waiting for the AI player to place the ships..."));
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

void KBattleshipWindow::slotAIReady()
{
	slotStatusMsg(i18n("Please place your ships. Use the \"Shift\" key to place the ships vertically."));
	m_placeable = true;
}

void KBattleshipWindow::slotAIShootsAt(const QPoint pos)
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

	slotStatusMsg(i18n("Enemy has shot. Shoot now."));
	slotChangeOwnFieldData(pos.x(), pos.y(), showstate);

	if(m_aiHits == 10 && m_stat->hit() != 10)
	{
		m_aiPlaying = false;
		m_shootable = false;
		slotChangeOwnPlayer("-");
		slotChangeEnemyPlayer("-");
		m_gameSingle->setText(i18n("S&ingle Player..."));
		m_gameNewServer->setEnabled(true);
		m_gameServerConnect->setEnabled(true);
		slotStatusMsg(i18n("You lost the game. :("));
		m_stat->slotAddEnemyWon();
		slotUpdateHighscore();
		m_view->drawEnemyShipsAI(m_enemyshiplist); // let's show ai player ships
		switch(KMessageBox::questionYesNo(this, i18n("Do you want to restart the game?"), QString::null, i18n("Restart"), i18n("Do Not Restart")))
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

void KBattleshipWindow::slotReceivedClientInformation(const QString &clientName, const QString &clientVersion, const QString &clientDescription, const QString &protocolVersion)
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
