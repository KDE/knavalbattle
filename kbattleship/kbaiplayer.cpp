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

#include "kbverticalstepstrategy.h"
#include "kbhorizontalstepstrategy.h"
#include "kbrandomshotstrategy.h"
#include "kbdiagonalwrapstrategy.h"

#include "kbaiplayer.moc"

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

    if(m_battleField != 0)
    {
	QRect rect = m_battleField->getEnemyRect();
	int grid = m_battleField->gridSize();
	m_fieldRect = QRect(0, 0, (rect.width() / grid) + 1, (rect.height() / grid) + 1);
    }

    m_ownShipList = ai_shiplist;
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

/* Initializes the master startegy which can create create child strategies */
void KBAIPlayer::chooseStrategy()
{
    if(m_masterStrategy != 0)
	delete m_masterStrategy;

    switch(m_randomSeq->getLong(MAX_STRAT_NUM))
    {
	case 0:
	    m_masterStrategy = new KBHorizontalStepStrategy();
	    //kdDebug() << "KBAIPlayer::chooseStrategy: HorizontalStep" << endl;
	    break;
	
	case 1:
	    m_masterStrategy = new KBVerticalStepStrategy();
	    //kdDebug() << "KBAIPlayer::chooseStrategy: VerticalStep" << endl;
	    break;
	
	case 2:
	    m_masterStrategy = new KBDiagonalWrapStrategy();
	    //kdDebug() << "KBAIPlayer::chooseStrategy: DiagonalWrap" << endl;
	    break;
	
	default:
	    m_masterStrategy = new KBRandomShotStrategy();
	    //kdDebug() << "KBAIPlayer::choosingStrategy: RandomShot(default)" << endl;
	    break;
    }
    
    m_masterStrategy->init(m_battleField, m_fieldRect);
}

/* Requests the next shot from the AI Player */
bool KBAIPlayer::slotRequestShot()
{
    if(m_masterStrategy != 0 && m_masterStrategy->hasMoreShots())
    {
	QPoint pos = m_masterStrategy->getNextShot();
	emit sigShootAt(pos);
        m_masterStrategy->shotAt(pos);

	return true;
    }
    return false;
}

/* Tests if the postion (x/y) is possible for a ship with length shiplen */
bool KBAIPlayer::shipPlaced(int shiplen, int x, int y, bool vertical)
{
    QRect ship = vertical ? QRect(x, y, 1, shiplen) : QRect(x, y, shiplen, 1);
    if(!m_fieldRect.contains(ship))
    {
//	kdDebug() << "     not on field" << endl;
	return false;
    }

    for(int s = MAX_SHIP_LEN; s > shiplen; s--)
    {
	for(int i = x; i < (x + ship.width()); i++)
	{
    	    QPoint p1(i, y - 1);
    	    QPoint p2(i, y + ship.height());
	    if(m_ships[s - 1].contains(p1) || m_ships[s - 1].contains(p2))
    	    {
	//	kdDebug() << "     collides with ship " << s << endl;
		return false;
    	    }
	}

	// test the fields left and right of the ship
	for (int i = y; i < (y + ship.height()); i++)
	{
    	    QPoint p1(x - 1, i);
    	    QPoint p2(x + ship.width(), i);
    	    if(m_ships[s - 1].contains(p1) || m_ships[s - 1].contains(p2))
    	    {
	//	kdDebug() << "     collides with ship " << s << endl;
		return false;
    	    }
	}
    }

		if (m_ownShipList->addNewShip(vertical, ship.x(), ship.y()))
		{
			m_ships[shiplen - 1] = ship;
			return true;
		}

//    kdDebug() << "     is ok, addShip(" << x1 << ", " << x2 << ", " << y1 << ", " << y2 << ", " << shiplen - 1 << ")" << endl;
    return false;
}
