/***************************************************************************
                              kbchooserstrategy.h
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

#ifndef KBCHOOSERSTRATEGY_H
#define KBCHOOSERSTRATEGY_H

#include <krandomsequence.h>

#include "kbstrategy.h"
#include "kbdestroyshipstrategy.h"

class KBChooserStrategy : public KBStrategy
{
public: 
	KBChooserStrategy(KBStrategy *parent = 0);
	virtual ~KBChooserStrategy();

	virtual void init(KBattleField *field, const QRect &field_rect);
	virtual const QPoint nextShot();
	virtual bool hasMoreShots();
	virtual void shotAt(const QPoint &pos);

private:
	bool advance();

	KBStrategy *m_child;
	KBDestroyShipStrategy *m_destroyer;
	KRandomSequence *m_random;

	bool m_destroying;
};

#endif
