/***************************************************************************
                            kbdiagonalshotstrategy.h
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

#ifndef KBDIAGONALSHOTSTRATEGY_H
#define KBDIAGONALSHOTSTRATEGY_H

#include "kbstrategy.h"

class KBDiagonalShotStrategy : public KBStrategy
{
    public: 
	enum Direction {LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN}; 
	KBDiagonalShotStrategy(KBStrategy *parent = 0);
	virtual ~KBDiagonalShotStrategy();

	virtual const QPoint nextShot();
	virtual bool hasMoreShots();
	virtual void shotAt(const QPoint &pos);
	virtual void startAt(int col, int row, Direction dir);
	virtual QPoint endPoint();

    private:
	bool advance();

	int m_row;
	int m_column;
	int m_vertical;
	int m_horizontal;
};

#endif
