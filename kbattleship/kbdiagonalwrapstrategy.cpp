/***************************************************************************
                           kbdiagonalwrapstrategy.cpp
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

#include <krandomsequence.h>
#include "kbdiagonalwrapstrategy.h"

KBDiagonalWrapStrategy::KBDiagonalWrapStrategy(KBStrategy *parent) : KBStrategy(parent)
{
    m_child = new KBDiagonalShotStrategy(this);

    if(parent == 0)
    {
	m_destroyer = new KBDestroyShipStrategy(this);
	m_destroying = false;
    }
    else
    {
	m_destroyer = 0L;
	m_destroying = false;
    }
}

KBDiagonalWrapStrategy::~KBDiagonalWrapStrategy()
{
    if(m_child != 0)
	delete m_child;
    if(m_destroyer != 0)
	delete m_destroyer;
}

void KBDiagonalWrapStrategy::init(KBattleField *field, const QRect &field_rect)
{
    KBStrategy::init(field, field_rect);
    KRandomSequence rand;
    
    m_column = (int) rand.getLong(m_fieldRect.width() - 1);
    m_row = (int) rand.getLong(m_fieldRect.height() - 1);

    switch(rand.getLong(4))
    {
	case 0:
	    m_direction = KBDiagonalShotStrategy::RIGHTDOWN;
	    m_row = 0;
	    break;

	case 1:
	    m_direction = KBDiagonalShotStrategy::RIGHTUP;
	    m_column = 0;
	    break;

	case 2:
	    m_direction = KBDiagonalShotStrategy::LEFTDOWN;
	    m_column = m_fieldRect.width() - 1; 
	    break;

	default:
	    m_direction = KBDiagonalShotStrategy::LEFTUP;
	    m_row = m_fieldRect.height() - 1;
	    break;
    }

    m_child->init(field, field_rect);
    m_child->startAt(m_column, m_row, m_direction);

    m_start = QPoint(m_column, m_row);

    if(m_destroyer != 0)
	m_destroyer->init(field, field_rect);
}

const QPoint KBDiagonalWrapStrategy::getNextShot()
{
    if(hasMoreShots())
    {
	if(m_destroying)
	    return m_destroyer->getNextShot();
	else if(m_child != 0)
	    return m_child->getNextShot();
    }

    return m_start;
}

bool KBDiagonalWrapStrategy::advance()
{
    switch(m_direction)
    {
	case KBDiagonalShotStrategy::RIGHTDOWN:
	    if(!advanceRightDown())
		return false;
	    break;
			
	case KBDiagonalShotStrategy::RIGHTUP:
	    if(!advanceRightUp())
		return false;
	    break;

	case KBDiagonalShotStrategy::LEFTDOWN:
	    if(!advanceLeftDown())
		return false;
	    break;

	case KBDiagonalShotStrategy::LEFTUP:
	    if(!advanceLeftUp())
		return false;
	    break;

	default:
	    break;
    }

    return m_child->hasMoreShots();
}

bool KBDiagonalWrapStrategy::hasMoreShots()
{
    if(m_parent == 0 && !m_destroying && m_prevShots->count() > 0)
    {
	QPoint *pos = m_prevShots->last();
	int state = m_battleField->getOwnState(pos->x(), pos->y());
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

    if(!m_child->hasMoreShots())
	return advance();

    return true;
}

void KBDiagonalWrapStrategy::shotAt(const QPoint &pos)
{
    m_prevShots->append(new QPoint(pos));

    if(m_child != 0)
	m_child->shotAt(pos);
}

bool KBDiagonalWrapStrategy::advanceRightDown()
{
    int col;

    if(m_column == 0 && m_row != 0)
    {
	// start next
	m_row = 0;
	m_column = (m_start.x() + 3) % m_fieldRect.width();
	m_child->startAt(m_column, m_row, m_direction);
		
	if(!m_child->hasMoreShots())
	{
	    col = 0;
	    m_child->startAt(col, m_row, m_direction);
	    while(!m_child->hasMoreShots())
	    {
		col++;
		if(col >= m_fieldRect.width())
		    return false;

		m_child->startAt(col, m_row, m_direction);
	    }
	    m_column = col;
	}
	m_start = QPoint(m_column, m_row);
    }
    else
    {
	//wrap;
	m_column = 0;
	m_row = m_child->endPoint().y();

	m_child->startAt(m_column, m_row, m_direction);
    }

    return true;
}

bool KBDiagonalWrapStrategy::advanceRightUp()
{
    int row;

    if(m_row == (m_fieldRect.height() - 1) && m_column != 0)
    {
	// start next
	m_column = 0;
	m_row = m_start.y() - 3;
	m_row = m_row < 0 ? m_row + m_fieldRect.height() : m_row;

	m_child->startAt(m_column, m_row, m_direction);
	
	if(!m_child->hasMoreShots())
	{
	    row = m_fieldRect.height() - 1;
	    m_child->startAt(m_column, row, m_direction);
	    while(!m_child->hasMoreShots())
	    {
		row--;
		if(row < 0)
		    return false;

		m_child->startAt(m_column, row, m_direction);
	    }
	    m_row = row;
	}
    	m_start = QPoint(m_column, m_row);
    }
    else
    {
	//wrap;
	m_row = m_fieldRect.height() - 1;
	m_column = m_child->endPoint().x();

	m_child->startAt(m_column, m_row, m_direction);
    }

    return true;
}

bool KBDiagonalWrapStrategy::advanceLeftDown()
{
    int row;

    if(m_row == 0 && m_column != (m_fieldRect.width()-1))
    {
	// start next
	m_column = m_fieldRect.width() - 1;
	m_row = (m_start.y() + 3) % m_fieldRect.height();

	m_child->startAt(m_column, m_row, m_direction);
		
	if(!m_child->hasMoreShots())
	{
	    row = 0;
	    m_child->startAt(m_column, row, m_direction);
	    while(!m_child->hasMoreShots())
	    {
		row++;
		if(row >= m_fieldRect.height())
		    return false;

		m_child->startAt(m_column, row, m_direction);
	    }
	    m_row = row;
	}
    	m_start = QPoint(m_column, m_row);
    }
    else
    {
	//wrap;
	m_row = 0;
	m_column = m_child->endPoint().x();

	m_child->startAt(m_column, m_row, m_direction);
    }

    return true;
}

bool KBDiagonalWrapStrategy::advanceLeftUp()
{
    int col;

    if(m_column == (m_fieldRect.width()-1) && m_row != (m_fieldRect.height()-1))
    {
	// start next
	m_row = m_fieldRect.height() - 1;
	m_column = m_start.x() - 3;
	m_column = m_column < 0 ? m_column + m_fieldRect.width() : m_column;

	m_child->startAt(m_column, m_row, m_direction);
	
	if(!m_child->hasMoreShots())
	{
	    col = m_fieldRect.width() - 1;
	    m_child->startAt(col, m_row, m_direction);
	    while(!m_child->hasMoreShots())
	    {
		col--;
		if(col < 0)
		    return false;

		m_child->startAt(col, m_row, m_direction);
	    }
	    m_column = col;
	}
	m_start = QPoint(m_column, m_row);
    }
    else
    {
	//wrap;
	m_column = m_fieldRect.width() - 1;
	m_row = m_child->endPoint().y();

	m_child->startAt(m_column, m_row, m_direction);
    }

    return true;
}
