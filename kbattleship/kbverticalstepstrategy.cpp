/***************************************************************************
                           kbverticalstepstrategy.cpp
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

#include <krandomsequence.h>
#include "kbverticalstepstrategy.h"

KBVerticalStepStrategy::KBVerticalStepStrategy(KBStrategy *parent) : KBStrategy(parent)
{
	m_child = 0;

	if(parent == 0) // only the master destroys ships
	{
		m_destroyer = new KBDestroyShipStrategy(this);
		m_destroying = false;
	}
	else
	{
		m_destroyer = 0;
		m_destroying = false;
	}
}

KBVerticalStepStrategy::~KBVerticalStepStrategy()
{
	delete m_child;
	delete m_destroyer;
}

void KBVerticalStepStrategy::init(KBattleField *field, const QRect &field_rect)
{
	KBStrategy::init(field, field_rect);
	KRandomSequence rand;
	m_column = (int) rand.getLong(m_fieldRect.width() - 1);
	m_row = (int) rand.getLong(m_fieldRect.height() - 1);
	m_start = QPoint(m_column, m_row);
	m_passes = 0;

	if(m_destroyer != 0)
		m_destroyer->init(field, field_rect);
}

const QPoint KBVerticalStepStrategy::nextShot()
{
	if(hasMoreShots())
	{
		if(m_destroying)
			return m_destroyer->nextShot();
		else if(m_passes == 0)
			return QPoint(m_column, m_row);
		else if(m_parent == 0)
			return m_child->nextShot();
	}

	return m_start;
}

bool KBVerticalStepStrategy::advance()
{
	int col, row;

	col = m_column;
	row = m_row;

	while(enemyFieldStateAt(col, row) == SHOT)
	{
		row += 2;
		if(row >= m_fieldRect.height())
		{
			row = m_row % 2;
			col = (col + 2) % m_fieldRect.width();
		}

		if(col == m_start.x() && row == m_start.y())
			break;
	}

	if(enemyFieldStateAt(col, row) != SHOT)
	{
		m_column = col;
		m_row = row;
		return true;
	}

	return false;
}

void KBVerticalStepStrategy::setStart(int col, int row)
{
	m_start = QPoint(col, row);
	m_column = col;
	m_row = row;
}

bool KBVerticalStepStrategy::hasMoreShots()
{
	if(m_parent != 0)
	{
		// Child Strategy
		if(m_passes == 0)
		{
			if(enemyFieldStateAt(m_column, m_row) != SHOT)
				return true;
			else if(advance())
				return true;
			else
			{
				m_passes++;
				return false;
			}
		}
		else
			return false;
	}
	else
	{
		// Parent Strategy
		if((!m_destroying) && m_prevShots->count() > 0)
		{
			QPoint* pos = m_prevShots->last();
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

		int x, y;
		switch(m_passes)
		{
			case 0:
				if(enemyFieldStateAt(m_column, m_row) != SHOT)
					return true;
				else if (advance())
					return true;

				m_passes++;
				m_child = new KBVerticalStepStrategy(this);
				m_child->init(m_battleField, m_fieldRect);

				x = (m_start.x() + 1) % m_fieldRect.width();
				y = (m_start.y() + 1) % m_fieldRect.height();
				m_child->setStart(x, y);

			case 1:
				if(m_child->hasMoreShots())
					return true;

				m_passes++;
				delete m_child;

				m_child = new KBVerticalStepStrategy(this);
				m_child->init(m_battleField, m_fieldRect);

				x = m_start.x();
				y = (m_start.y() + 1) % m_fieldRect.height();
				m_child->setStart(x, y);

			case 2:
				if(m_child->hasMoreShots())
					return true;

				m_passes++;
				delete m_child;

				m_child = new KBVerticalStepStrategy(this);
				m_child->init(m_battleField, m_fieldRect);

				x = (m_start.x() + 1) % m_fieldRect.width();
				y = (m_start.y() + 2) % m_fieldRect.height();
				m_child->setStart(x, y);

			case 3:
				if(m_child->hasMoreShots())
					return true;

				m_passes++;

			default:
				return false;
		}
	}
}

void KBVerticalStepStrategy::shotAt(const QPoint &pos)
{
	m_prevShots->append(new QPoint(pos));
	if(m_child != 0)
		m_child->shotAt(pos);
}
