/***************************************************************************
                                 kbstrategy.h
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

#ifndef KBSTRATEGY_H
#define KBSTRATEGY_H

#include <qlist.h>
#include <qpoint.h>
#include "kbattlefield.h"

class KBStrategy
{
    public:
	enum{FREE, SHOT, SHIP};
	KBStrategy(KBStrategy *parent = 0);
	virtual ~KBStrategy();

	virtual const QPoint getNextShot() = 0;
        virtual void shotAt(const QPoint &pos);
	virtual void init(KBattleField *field, const QRect &field_rect);
	virtual bool hasMoreShots() = 0;

    protected:
	virtual QList<QPoint> *getMasterShotList();
	virtual int enemyFieldStateAt(int x, int y);

	QRect m_fieldRect;
	QList<QPoint> *m_prevShots;

	KBattleField *m_battleField;
	KBStrategy *m_parent;

};

#endif
