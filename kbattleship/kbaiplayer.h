/***************************************************************************
                                 kbaiplayer.h
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

#ifndef KBAIPLAYER_H
#define KBAIPLAYER_H

#define MAX_SHIP_LEN 4

#include <qobject.h>

#include <krandomsequence.h>

#include "kbstrategy.h"
#include "kbattlefield.h"
#include "kshiplist.h"

class KBAIPlayer : public QObject
{
    Q_OBJECT
    public: 
	KBAIPlayer();
	~KBAIPlayer();
	
	void init(KBattleField *battle_field, KShipList *ai_shiplist);

    public slots:
	void slotRestart();
	bool slotRequestShot();

    protected:
	KBStrategy *m_masterStrategy;
	KShipList *m_ownShipList;
	KBattleField *m_battleField;
	KRandomSequence *m_randomSeq;

	QRect m_fieldRect;
	QRect m_ships[MAX_SHIP_LEN];

	void chooseStrategy();
	void addShips();
	bool shipPlaced(int shiplen, int x, int y, bool vertical);

    signals:
	void shootAt(const QPoint pos);
	void ready();
};

#endif
