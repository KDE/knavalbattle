/***************************************************************************
                                kbaiplayer.cpp
                                  ----------
    Developers: (c) 2001 Kevin Krammer <kevin.krammer@gmx.at>
		(c) 2001 Nikolas Zimmermann <wildfox@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kapp.h>
#include <kdebug.h>

#include "kbchooserstrategy.h"

#include "kbaiplayer.moc"

#define MAX_SHIP_LEN 4

KBAIPlayer::KBAIPlayer()
{
	m_ownShipList = 0;
	m_battleField = 0;
	m_masterStrategy = 0;
	m_randomSeq = new KRandomSequence(KApplication::random());
}

KBAIPlayer::~KBAIPlayer()
{
	if(m_masterStrategy != 0)
		delete m_masterStrategy;

	if(m_randomSeq != 0)
		delete m_randomSeq;
}

void KBAIPlayer::init(KBattleField *battle_field, KShipList *ai_shiplist)
{
	m_battleField = battle_field;
	m_ownShipList = ai_shiplist;

	if(m_battleField != 0)
	{
		QRect rect = m_battleField->enemyRect();
		int grid = m_battleField->gridSize();
		m_fieldRect = QRect(0, 0, (rect.width() / grid) + 1, (rect.height() / grid) + 1);
	}
}

void KBAIPlayer::slotRestart()
{
	if(m_randomSeq == 0 || m_ownShipList == 0 || m_battleField == 0)
		return;

	addShips();
	chooseStrategy();
	emit sigReady();
}

void KBAIPlayer::addShips()
{
	m_ownShipList->clear();

	for(int shiplen = MAX_SHIP_LEN; shiplen >= 1; shiplen--)
	{
		int x, y;
		bool vertical;

		do
		{
			x = (int) m_randomSeq->getLong(m_fieldRect.width() - 1);
			y = (int) m_randomSeq->getLong(m_fieldRect.height() - 1);
			vertical = m_randomSeq->getBool();
		}
		while(!shipPlaced(shiplen, x, y, vertical));
	}
}

void KBAIPlayer::chooseStrategy()
{
	if(m_masterStrategy != 0)
		delete m_masterStrategy;

	m_masterStrategy = new KBChooserStrategy();
	m_masterStrategy->init(m_battleField, m_fieldRect);
}

bool KBAIPlayer::slotRequestShot()
{
	if(m_masterStrategy != 0 && m_masterStrategy->hasMoreShots())
	{
		QPoint pos = m_masterStrategy->nextShot();
		emit sigShootAt(pos);
		m_masterStrategy->shotAt(pos);
		return true;
	}
	return false;
}

bool KBAIPlayer::shipPlaced(int shiplen, int x, int y, bool vertical)
{
	QRect ship = vertical ? QRect(x, y, 1, shiplen) : QRect(x, y, shiplen, 1);
	return m_ownShipList->addNewShip(vertical, ship.x(), ship.y());
}
