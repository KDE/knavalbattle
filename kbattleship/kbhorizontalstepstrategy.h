/***************************************************************************
                            kbhorizontalstepstrategy.h
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

#ifndef KBHORIZONTALSTEPSTRATEGY_H
#define KBHORIZONTALSTEPSTRATEGY_H

#include "kbstrategy.h"
#include "kbdestroyshipstrategy.h"

class KBHorizontalStepStrategy : public KBStrategy
{
    public: 
	KBHorizontalStepStrategy(KBStrategy *parent = 0);
	virtual ~KBHorizontalStepStrategy();

	virtual void init(KBattleField *field, const QRect &field_rect);
	virtual const QPoint getNextShot();
	virtual bool hasMoreShots();
	virtual void shotAt(const QPoint &pos);

    protected:
	bool advance();
	void setStart(int col, int row);

	int m_row;
	int m_column;
	int m_passes;

	QPoint m_start;
	KBHorizontalStepStrategy *m_child;
	KBDestroyShipStrategy* m_destroyer;
	bool m_destroying;
};

#endif
