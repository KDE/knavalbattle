/***************************************************************************
                            kbdestroyshipstratgey.h
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

#ifndef KBDESTROYSHIPSTRATEGY_H
#define KBDESTROYSHIPSTRATEGY_H

#include "kbstrategy.h"

class KBDestroyShipStrategy : public KBStrategy
{
public:
	KBDestroyShipStrategy(KBStrategy *parent = 0);

	virtual void init(KBattleField *field, const QRect &field_rect);
	virtual const QPoint nextShot();
	virtual bool hasMoreShots();
	virtual void shotAt(const QPoint &pos);

	virtual void destroyShipAt(const QPoint pos);

private:
	enum {NODIR, VERTICAL, HORIZONTAL};

	bool m_working;
	QPoint m_start;

	int m_column;
	int m_row;

	int m_direction;

	virtual bool searchUp();
	virtual bool searchDown();
	virtual bool searchLeft();
	virtual bool searchRight();

	virtual bool shipDestroyed();
	virtual void markBorderingFields();
};

#endif
