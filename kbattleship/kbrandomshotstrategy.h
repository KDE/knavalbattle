/***************************************************************************
                             kbrandomshotstrategy.h
                                  ----------
    Developers: (c) 2001 Kevin Krammer <kevin.krammer@gmx.at>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KBRANDOMSHOTSTRATEGY_H
#define KBRANDOMSHOTSTRATEGY_H

#include <krandomsequence.h>

#include "kbstrategy.h"
#include "kbdestroyshipstrategy.h"

class KBRandomShotStrategy : public KBStrategy
{
    public: 
	KBRandomShotStrategy(KBStrategy *parent = 0);
	virtual ~KBRandomShotStrategy();

	virtual void init(KBattleField *field, const QRect &field_rect);
	virtual const QPoint nextShot();
	virtual bool hasMoreShots();
	virtual void shotAt(const QPoint &pos);

    private:
	bool advance();

	int m_row;
	int m_column;

	KRandomSequence m_randomSeq;
	KBDestroyShipStrategy *m_destroyer;
	bool m_destroying;
};

#endif
