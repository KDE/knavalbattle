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

KBattleshipApp::KBattleshipApp(QWidget *, const char *name) : KMainWindow(0, name)
{
    setMinimumSize(680, 480);
    connection = 0;
}

KBattleshipApp::~KBattleshipApp()
{
    saveOptions();
}

void KBattleshipApp::init()
{
    haveCS = false;
    place = false;
    config = kapp->config();
    initStatusBar();
    initActions();
    readOptions();
    initView();
    initChat();
    initSound();
    initShipPlacing();
}

void KBattleshipApp::initActions()
{
    gameQuit = new KAction(i18n("&Quit"), "exit", Key_F9, this, SLOT(slotGameQuit()), actionCollection(), "gamequit");
    gameServerConnect = new KAction(i18n("&Connect to server"), "connect_no", Key_F2, this, SLOT(slotServerConnect()), actionCollection(), "serverconnect");
    gameNewServer = new KAction(i18n("&Start server"), "network", Key_F3, this, SLOT(slotNewServer()), actionCollection(), "newserver");
    configSound = new KToggleAction(i18n("&Play sounds"), 0, this, SLOT(slotConfigSound()), actionCollection(), "configsound");
    viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());

    createGUI();
}

void KBattleshipApp::initSound()
{
    new Arts::Dispatcher;
    sound = new KBattleshipSound();
    if(!sound->initSoundServer())
	configSound->setChecked(false);
    else   
	slotConfigSound();
}

void KBattleshipApp::initChat()
{
    connect(chat, SIGNAL(sendMessage(QString)), this, SLOT(sendChatMessage(QString)));
}

void KBattleshipApp::initShipPlacing()
{
    connect(ownshiplist, SIGNAL(ownFieldDataChanged(int, int, int)), this, SLOT(changeOwnFieldData(int, int, int)));
}

void KBattleshipApp::initStatusBar()
{
    ownNickname = "-";
    enemyNickname = "-";
    statusBar()->insertItem(i18n("     Player 1: %1     ").arg(ownNickname), ID_PLAYER_OWN, 0, true);
    statusBar()->insertItem(i18n("     Player 2: %1     ").arg(enemyNickname), ID_PLAYER_ENEMY, 0, true);
    statusBar()->insertItem(i18n("Ready"), ID_STATUS_MSG, 1);
    statusBar()->setItemAlignment(ID_STATUS_MSG, AlignLeft);
}

void KBattleshipApp::initView()
{
    splitV = new QSplitter(QSplitter::Vertical, this);
    splitH = new QSplitter(QSplitter::Horizontal, splitV);

    view = new KBattleshipView(splitH);
    chat = new KChatWidget(splitV);
    stat = new KStatDialog(splitH);
    
    chat->acceptMsg(false);

    ownshiplist = new KShipList();
    enemyshiplist = new KShipList();

    view->startDrawing();

    setCentralWidget(splitV);

    connect(ownshiplist, SIGNAL(lastShipAdded()), this, SLOT(sendShipList()));    
    connect(view, SIGNAL(enemyFieldClicked(int, int)), this, SLOT(enemyClick(int, int)));
    connect(view, SIGNAL(ownFieldClicked(int, int, int)), this, SLOT(placeShip(int, int, int)));
    
    setCaption(i18n("KBattleship (beta2)"), false);
}

void KBattleshipApp::enemyClick(int fieldx, int fieldy)
{
    if(haveCS && connection != 0)
    {
        if(connection->writeable() && connection->gotEnemyList() && connection->sendEnemyList())
        {
            if(view->getEnemyFieldState(fieldx, fieldy) == KBattleField::FREE)
	    {
		int showstate;
		int enemystate = enemyshiplist->getXYShipType(fieldx, fieldy);
	    
	        stat->setShot();
	    
	        if(enemystate == 99)
	        {
	            stat->setWater();
	            showstate = KBattleField::WATER;
	        }
	        else
	        {
	            stat->setHit();
	            showstate = KBattleField::HIT;
	        }
		
	        changeEnemyFieldData(fieldx, fieldy, showstate);

	        if(stat->getHit() != 10)
	        {
	            sendMessage(fieldx, fieldy, showstate);
	            slotStatusMsg(i18n("Waiting for enemy to shoot.."));
	        }
	        else
	        {
	            sendMessage(fieldx, fieldy, showstate, true);
	            slotStatusMsg(i18n("You won the game :)"));
	            if(connection->getType() == KonnectionHandling::SERVER)
		        resetServer(false);
	            else if(connection->getType() == KonnectionHandling::CLIENT)
		        resetClient(false);
		}
	    }
	}
    }
}

void KBattleshipApp::placeShip(int fieldx, int fieldy, int button)
{
    if(haveCS && connection != 0)
    {
        if(connection->haveEnemy())
	{
	    if(connection->getType() == KonnectionHandling::CLIENT)
	    {
		if(place)
	        {
		    if(ownshiplist->canAddShips())
			ownshiplist->addNewShip(button, fieldx, fieldy);
            	}
	    }
	    else if(connection->getType() == KonnectionHandling::SERVER)
	    {
		if(ownshiplist->canAddShips())
		    ownshiplist->addNewShip(button, fieldx, fieldy);
	    }
        }
    }
}

void KBattleshipApp::sendShipList()
{
    KMessage *msg = new KMessage(KMessage::SHIPLIST);
    msg->addField(QString("fieldx1s1"), QString::number(ownshiplist->returnX1Ship(1)));
    msg->addField(QString("fieldx2s1"), QString::number(ownshiplist->returnX2Ship(1)));
    msg->addField(QString("fieldx1s2"), QString::number(ownshiplist->returnX1Ship(2)));
    msg->addField(QString("fieldx2s2"), QString::number(ownshiplist->returnX2Ship(2)));
    msg->addField(QString("fieldx1s3"), QString::number(ownshiplist->returnX1Ship(3)));
    msg->addField(QString("fieldx2s3"), QString::number(ownshiplist->returnX2Ship(3)));
    msg->addField(QString("fieldx1s4"), QString::number(ownshiplist->returnX1Ship(4)));
    msg->addField(QString("fieldx2s4"), QString::number(ownshiplist->returnX2Ship(4)));
    msg->addField(QString("fieldy1s1"), QString::number(ownshiplist->returnY1Ship(1)));
    msg->addField(QString("fieldy2s1"), QString::number(ownshiplist->returnY2Ship(1)));
    msg->addField(QString("fieldy1s2"), QString::number(ownshiplist->returnY1Ship(2)));
    msg->addField(QString("fieldy2s2"), QString::number(ownshiplist->returnY2Ship(2)));
    msg->addField(QString("fieldy1s3"), QString::number(ownshiplist->returnY1Ship(3)));
    msg->addField(QString("fieldy2s3"), QString::number(ownshiplist->returnY2Ship(3)));
    msg->addField(QString("fieldy1s4"), QString::number(ownshiplist->returnY1Ship(4)));
    msg->addField(QString("fieldy2s4"), QString::number(ownshiplist->returnY2Ship(4)));
    msg->addReady();

    sendMessage(msg);

    if(connection->getType() == KonnectionHandling::SERVER)
	slotStatusMsg(i18n("Waiting for other player to place the ships..."));
    else if(connection->getType() == KonnectionHandling::CLIENT)
	slotStatusMsg(i18n("Waiting for other player to start the match..."));
}

void KBattleshipApp::sendMessage(int fieldx, int fieldy, int state, bool won)
{
    if(haveCS)
    {
	KMessage *msg = new KMessage(KMessage::ANSWER_SHOOT);
	msg->addField(QString("fieldx"), QString::number(fieldx));
	msg->addField(QString("fieldy"), QString::number(fieldy));
	msg->addField(QString("fieldstate"), QString::number(state));
	msg->addReady();
	
	if(won)
	    msg->addWinner();
	
        if(connection->getType() == KonnectionHandling::SERVER)
	    kbserver->sendMessage(msg);
        else if(connection->getType() == KonnectionHandling::CLIENT)
            kbclient->sendMessage(msg);
    }
}

void KBattleshipApp::sendMessage(KMessage *msg)
{
    if(haveCS)
    {
	if(connection->getType() == KonnectionHandling::SERVER)
    	    kbserver->sendMessage(msg);
        else if(connection->getType() == KonnectionHandling::CLIENT)
            kbclient->sendMessage(msg);
    }
}

void KBattleshipApp::sendChatMessage(QString text)
{
    if(haveCS && connection != 0)
    {
        if(connection->haveEnemy())
        {
	    bool currentstate = connection->writeable();
	    bool serverallow = false, clientallow = false, isserver = false, isclient = false;
	    if(!currentstate)
    	    {	
		if(connection->getType() == KonnectionHandling::SERVER)
                {
	    	    serverallow = true;
    		    isserver = true;
	    	}
    		else if(connection->getType() == KonnectionHandling::CLIENT)
                {
                    clientallow = true;
    		    isclient = true;
	        }
    	    }
    	    else
	    {
    		if(connection->getType() == KonnectionHandling::SERVER)
		    isserver = true;
		else if(connection->getType() == KonnectionHandling::CLIENT)
		    isclient = true;
    	    }

    	    if(serverallow)
		kbserver->allowWrite();
	    else if(clientallow)
    	        kbclient->allowWrite();

    	    KMessage *msg = new KMessage(KMessage::CHAT);
            msg->chatMessage(ownNickname, text);

	    if(isserver)
    		kbserver->sendMessage(msg);
	    else if(isclient)
    	        kbclient->sendMessage(msg);

    	    if(serverallow)
	        kbserver->forbidWrite();
	    else if(clientallow)
    	        kbclient->forbidWrite();
        }
    }
}

void KBattleshipApp::clientRestart()
{
    slotStatusMsg(i18n("Waiting for other player to place the ships..."));
    place = false;
    view->clearField();
    stat->clear();
    deleteLists();
    connection->clear();
}

void KBattleshipApp::saveOptions()
{
    config->setGroup("General");
    config->writeEntry("ShowStatusbar", viewStatusBar->isChecked());
    if(!sound->serverError())
	config->writeEntry("PlaySounds", configSound->isChecked());
    config->sync();
}

void KBattleshipApp::readOptions()
{
    config->setGroup("General");

    bool bViewStatusbar = config->readBoolEntry("ShowStatusbar", true);
    viewStatusBar->setChecked(bViewStatusbar);
    slotViewStatusBar();
    
    bool bConfigSound = config->readBoolEntry("PlaySounds", true);
    configSound->setChecked(bConfigSound);    
}

void KBattleshipApp::slotGameQuit()
{
    kapp->quit();
}

void KBattleshipApp::slotServerConnect()
{
    if(!haveCS)
    {
        slotStatusMsg(i18n("Loading Connect-Server dialog..."));
	
	client = new KClientDialog();
	haveCS = true;
        connect(client, SIGNAL(connectServer()), this, SLOT(connectToBattleshipServer()));
	connect(client, SIGNAL(cancelConnect()), this, SLOT(resetConnection()));
	client->show();

        slotStatusMsg(i18n("Ready"));
    }
    else
	resetClient();
}

void KBattleshipApp::resetClient(bool status)
{
    if(!status)
    {
	KMessage *msg = new KMessage(KMessage::REPLAY);
	switch(KMessageBox::questionYesNo(this, "Do you want to ask to server restarting the game?"))
	{
	    case KMessageBox::Yes:
		slotStatusMsg(i18n("Waiting for an answer..."));
    		view->clearField();
    		stat->clear();
		msg->addReplayRequest();
		if(!connection->writeable())
		{
		    kbclient->allowWrite();
		    sendMessage(msg);
		    kbclient->forbidWrite();
		}
		else
		    sendMessage(msg);
		place = false;
		connection->clear();
		break;
	
	    case KMessageBox::No:
		delete msg;
    	        resetConnection();
    		place = false;
		slotStatusMsg(i18n("Ready"));
    		slotChangeOwnPlayer("-");
    		slotChangeEnemyPlayer("-");
		gameServerConnect->setText("&Connect to server");
		gameNewServer->setEnabled(true);
		view->clearField();
		stat->clear();
		connection->clear();
		delete kbclient;
		kbclient = 0;
		break;
        }
    }
    else
    {
        slotChangeOwnPlayer("-");
        slotChangeEnemyPlayer("-");
	gameServerConnect->setText("&Connect to server");
        gameNewServer->setEnabled(true);
	view->clearField();
	stat->clear();
	chat->clear();
	resetConnection();
	connection->clear();
	// Niko Z:
	// avoid crashes when deleting ### TODO
	// correct deleting! (we don't wanna leak)
	delete kbclient;
	kbclient = 0;
	slotStatusMsg(i18n("Ready"));
    }
    deleteLists();
}

void KBattleshipApp::askReplay()
{
    switch(KMessageBox::questionYesNo(this, "The client asks for restarting the game. Do you accept?"))
    {
	case KMessageBox::Yes:
	    slotStatusMsg(i18n("Please place your ships"));
	    view->clearField();
    	    stat->clear();
	    connection->clear();
            break;
	    
	case KMessageBox::No:
    	    slotStatusMsg(i18n("Ready"));
	    gameNewServer->setText("&Start server");
	    gameServerConnect->setEnabled(true);
	    view->clearField();
    	    stat->clear();
	    chat->clear();
    	    resetConnection();
	    connection->clear();
	    delete kbserver;
	    kbserver = 0;
            break;
    }
    deleteLists();
}

void KBattleshipApp::resetServer(bool status)
{
    if(!status)
    {
    	KMessage *msg = new KMessage(KMessage::REPLAY);
	switch(KMessageBox::questionYesNo(this, "Do you want to restart the game?"))
	{
	    case KMessageBox::Yes:
    		view->clearField();
    		stat->clear();
		slotStatusMsg(i18n("Please place your ships"));
		msg->addReplayRequest();
		if(!connection->writeable())
		{
		    kbserver->allowWrite();
		    sendMessage(msg);
		    kbserver->forbidWrite();
		}
		else
		    sendMessage(msg);
		connection->clear();
		break;
	
	    case KMessageBox::No:
		delete msg;
	        resetConnection();
            	place = false;
	    	slotStatusMsg(i18n("Ready"));
    	        slotChangeOwnPlayer("-");
		slotChangeEnemyPlayer("-");
        	gameNewServer->setText("&Start server");
		gameServerConnect->setEnabled(true);
		view->clearField();
    		stat->clear();
		chat->clear();
		connection->clear();
		delete kbserver;
		kbserver = 0;
		break;
        }
    }
    else
    {
        slotChangeOwnPlayer("-");
        slotChangeEnemyPlayer("-");
        gameNewServer->setText("&Start server");
        gameServerConnect->setEnabled(true);
	view->clearField();
	stat->clear();
	chat->clear();
	resetConnection();
	connection->clear();
	delete kbserver;
	kbserver = 0;
        slotStatusMsg(i18n("Ready"));
    }
    deleteLists();
}

void KBattleshipApp::deleteLists()
{
    ownshiplist->clear();
    enemyshiplist->clear();
}

void KBattleshipApp::slotNewServer()
{
    if(!haveCS)
    {
        slotStatusMsg(i18n("Loading Start-Server dialog..."));

        server = new KServerDialog();
        haveCS = true;
        connect(server, SIGNAL(startServer()), this, SLOT(startBattleshipServer()));
        connect(server, SIGNAL(cancelServer()), this, SLOT(resetConnection()));
        server->show();
            
        slotStatusMsg(i18n("Ready"));
    }
    else
        resetServer();
}

void KBattleshipApp::sendGreet()
{
    KMessage *msg = new KMessage(KMessage::GREET);
    msg->addField(QString("nickname"), ownNickname);
    sendMessage(msg);
}

void KBattleshipApp::startBattleshipServer()
{
    gameNewServer->setText("&Stop server");
    gameServerConnect->setEnabled(false);
    slotStatusMsg(i18n("Waiting for a player..."));
    kbserver = new KBattleshipServer((server->getPort()).toInt());
    ownNickname = server->getNickname();
    chat->setNickname(ownNickname);
    slotChangeOwnPlayer(ownNickname);
    delete server;
    if(connection == 0)
    {
	connection = new KonnectionHandling(this, kbserver);
	connect(connection, SIGNAL(newPlayer(bool)), chat, SLOT(acceptMsg(bool)));
	connect(connection, SIGNAL(askReplay()), this, SLOT(askReplay()));
	connect(connection, SIGNAL(abortGame()), this, SLOT(deleteLists()));
	connect(connection, SIGNAL(abortGameStrict(bool)), this, SLOT(resetServer(bool)));
	connect(connection, SIGNAL(serverFailure(bool)), this, SLOT(resetServer(bool)));
	connect(connection, SIGNAL(giveEnemyName()), this, SLOT(sendGreet())); 
	connect(connection, SIGNAL(enemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
	connect(connection, SIGNAL(statusBarMessage(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
	connect(connection, SIGNAL(ownFieldDataChanged(int, int, int)), this, SLOT(changeOwnFieldData(int, int, int)));
	connect(connection, SIGNAL(gotChatMessage(QString, QString)), chat, SLOT(receivedMessage(QString, QString)));
	connect(connection, SIGNAL(gotEnemyShipList(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString)), this, SLOT(gotEnemyShipList(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString)));
    }
    else
	connection->updateInternal(kbserver);
    kbserver->start();
}

void KBattleshipApp::changeOwnFieldData(int fieldx, int fieldy, int type)
{
    view->changeOwnFieldData(fieldx, fieldy, type);
    
    switch(connection->getType())
    {
	case KonnectionHandling::SERVER:
	    switch(type)
	    {
		case KBattleField::WATER:
	    	    sound->playSound(KBattleshipSound::PLAYER_SHOOT_WATER);
		    break;
		    
		case KBattleField::HIT:
		    sound->playSound(KBattleshipSound::PLAYER1_SHOOT_HIT);
		    break;
		
		case KBattleField::DEATH:
		    sound->playSound(KBattleshipSound::SHIP_SINK);
		    break;
	    }
	    break;
	    
	case KonnectionHandling::CLIENT:
	    switch(type)
	    {
		case KBattleField::WATER:
		    sound->playSound(KBattleshipSound::PLAYER_SHOOT_WATER);
		    break;
		    
		case KBattleField::HIT:
		    sound->playSound(KBattleshipSound::PLAYER2_SHOOT_HIT);
		    break;
		
		case KBattleField::DEATH:
		    sound->playSound(KBattleshipSound::SHIP_SINK);
		    break;
	    }
	    break;
    }
}

int KBattleshipApp::getOwnFieldType(int fieldx, int fieldy)
{
    return ownshiplist->getXYShipType(fieldx, fieldy);
}

int KBattleshipApp::getEnemyFieldType(int fieldx, int fieldy)
{
    return enemyshiplist->getXYShipType(fieldx, fieldy);
}

void KBattleshipApp::changeEnemyFieldData(int fieldx, int fieldy, int type)
{    
    view->changeEnemyFieldData(fieldx, fieldy, type);
    
    if(type == KBattleField::HIT)
    {
	if(enemyshiplist->getXYShipType(fieldx, fieldy) != 0 && enemyshiplist->getXYShipType(fieldx, fieldy) != 99)
	{
	    KShip *ship = enemyshiplist->getXYShip(fieldx, fieldy);
	    typedef QValueList<int> DeathValueList;
	    DeathValueList deathList;
    	    bool xokay = true, yokay = true;
	    int tempy = 0, tempx = 0;

	    if(ship->shipystart() == ship->shipystop() && ship->shipxstart() != ship->shipxstop())
    	    {
		for(tempx = ship->shipxstart(); tempx <= ship->shipxstop(); tempx++)
		{
		    if(view->getEnemyFieldState(tempx, fieldy) == KBattleField::HIT)
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
		    if(view->getEnemyFieldState(fieldx, tempy) == KBattleField::HIT)
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
                for(it = deathList.begin(); it != deathList.end(); it++)
                    view->changeEnemyFieldData(*it, fieldy, KBattleField::DEATH);
            }
            else if(yokay)
            {
                DeathValueList::Iterator it;
                for(it = deathList.begin(); it != deathList.end(); it++)
                    view->changeEnemyFieldData(fieldx, *it, KBattleField::DEATH);
            }
        }
        else if(enemyshiplist->getXYShipType( fieldx, fieldy ) == 0)
            view->changeEnemyFieldData(fieldx, fieldy, KBattleField::DEATH);
    }
}

void KBattleshipApp::changeConnectText()
{
    resetClient();
}

void KBattleshipApp::changeStartText()
{
    resetServer();
}

void KBattleshipApp::connectToBattleshipServer()
{
    if(client->getHost() != "")
    {
	kbclient = new KBattleshipClient(client->getHost(), (client->getPort()).toInt());
	ownNickname = client->getNickname();
	chat->setNickname(ownNickname);
	slotChangeOwnPlayer(ownNickname);
	delete client;
        gameServerConnect->setText("Dis&connect from server");
        gameNewServer->setEnabled(false);
	slotStatusMsg(i18n("Waiting for other player to place the ships..."));
	if(connection == 0)
	{
	    connection = new KonnectionHandling(this, kbclient);
	    connect(kbclient, SIGNAL(connected()), this, SLOT(sendGreet()));
	    connect(connection, SIGNAL(changeConnectText()), this, SLOT(changeConnectText()));
	    kbclient->init();
	    connect(connection, SIGNAL(newPlayer(bool)), chat, SLOT(acceptMsg(bool)));
	    connect(connection, SIGNAL(clientRestart()), this, SLOT(clientRestart()));
	    connect(connection, SIGNAL(enemyNickname(const QString &)), this, SLOT(slotChangeEnemyPlayer(const QString &)));
    	    connect(connection, SIGNAL(statusBarMessage(const QString &)), this, SLOT(slotStatusMsg(const QString &)));
	    connect(connection, SIGNAL(ownFieldDataChanged(int, int, int)), this, SLOT(changeOwnFieldData(int, int, int)));
	    connect(connection, SIGNAL(setPlaceable()), this, SLOT(setPlaceable()));
	    connect(connection, SIGNAL(abortGame()), this, SLOT(deleteLists()));
	    connect(connection, SIGNAL(abortGameStrict(bool)), this, SLOT(resetClient(bool)));
	    connect(connection, SIGNAL(gotChatMessage(QString, QString)), chat, SLOT(receivedMessage(QString, QString)));
	    connect(connection, SIGNAL(gotEnemyShipList(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString)), this, SLOT(gotEnemyShipList(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString)));
	}
	else
	{
	    connection->updateInternal(kbclient);
	    kbclient->init();
	}
    }
    else
    {
	KMessageBox::error(this, i18n("You forgot to enter a host!"));
	resetConnection();	
    }
}

void KBattleshipApp::setPlaceable()
{
    place = true;
}

void KBattleshipApp::gotEnemyShipList(QString fieldX1S1, QString fieldY1S1, QString fieldX2S1, QString fieldY2S1, QString fieldX1S2, QString fieldY1S2, QString fieldX2S2, QString fieldY2S2, QString fieldX1S3, QString fieldY1S3, QString fieldX2S3, QString fieldY2S3, QString fieldX1S4, QString fieldY1S4, QString fieldX2S4, QString fieldY2S4)
{
    enemyshiplist->addShip1(fieldX1S1.toInt(), fieldX2S1.toInt(), fieldY1S1.toInt(), fieldY2S1.toInt());
    enemyshiplist->addShip2(fieldX1S2.toInt(), fieldX2S2.toInt(), fieldY1S2.toInt(), fieldY2S2.toInt());
    enemyshiplist->addShip3(fieldX1S3.toInt(), fieldX2S3.toInt(), fieldY1S3.toInt(), fieldY2S3.toInt());
    enemyshiplist->addShip4(fieldX1S4.toInt(), fieldX2S4.toInt(), fieldY1S4.toInt(), fieldY2S4.toInt());
}

void KBattleshipApp::slotConfigSound()
{
    if(!configSound->isChecked())
        sound->turnOff();
    else
        sound->turnOn();
}

void KBattleshipApp::slotViewStatusBar()
{
    if(!viewStatusBar->isChecked())
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
