/***************************************************************************
                             kbchooserstrategy.cpp
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

#include "kbchooserstrategy.h"

#include <kapplication.h>

#include "kbrandomshotstrategy.h"
#include "kbdiagonalwrapstrategy.h"
#include "kbhorizontalstepstrategy.h"
#include "kbverticalstepstrategy.h"

#define MAX_CHILD_NUM 4

KBChooserStrategy::KBChooserStrategy(KBStrategy *parent) : KBStrategy(parent)
{
	m_destroyer = new KBDestroyShipStrategy(this);
	m_destroying = false;

	m_child = 0;

	m_random = new KRandomSequence(KApplication::random());
}

KBChooserStrategy::~KBChooserStrategy()
{
	if(m_destroyer != 0)
		delete m_destroyer;

	if(m_child != 0)
		delete m_child;

	delete m_random;
}

void KBChooserStrategy::init(KBattleField *field, const QRect &field_rect)
{
	KBStrategy::init(field, field_rect);

	if(m_destroyer != 0)
		m_destroyer->init(field, field_rect);

	advance();
}

const QPoint KBChooserStrategy::nextShot()
{
	if(hasMoreShots())
	{
		if(m_destroying)
			return m_destroyer->nextShot();
		else if(advance())
			return m_child->nextShot();
	}

	return QPoint(0, 0);
}

bool KBChooserStrategy::advance()
{
	if(!m_destroying && m_prevShots->count() % 5 == 0)
	{
		delete m_child;

		switch(m_random->getLong(MAX_CHILD_NUM))
		{
			case 0:
				m_child = new KBVerticalStepStrategy(this);
				break;

			case 1:
				m_child = new KBHorizontalStepStrategy(this);
				break;

			case 2:
				m_child = new KBDiagonalWrapStrategy(this);
				break;

			default:
				m_child = new KBRandomShotStrategy(this);
				break;
		}

		m_child->init(m_battleField, m_fieldRect);
	}

	return true;
}

bool KBChooserStrategy::hasMoreShots()
{
	if(m_parent == 0)
	{
		if((!m_destroying) && m_prevShots->count() > 0)
		{
			QPoint *pos = m_prevShots->last();
			int state = m_battleField->ownState(pos->x(), pos->y());
			if(state == KBattleField::HIT)
			{
				m_destroying = true;
				m_destroyer->destroyShipAt(*pos);
			}
		}

		if(m_destroying)
		{
			if(m_destroyer->hasMoreShots())
				return true;
			else
				m_destroying = false;
		}
	}

	for(int row = 0; row < m_fieldRect.height(); row++)
	{
		for(int col = 0; col < m_fieldRect.width(); col++)
		{
			if(enemyFieldStateAt(col, row) != SHOT)
				return true;
		}
	}

	return false;
}

void KBChooserStrategy::shotAt(const QPoint &pos)
{
	m_prevShots->append(new QPoint(pos));
	m_child->shotAt(pos);
}
