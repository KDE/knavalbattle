/***************************************************************************
                            kbrandomshotstrategy.cpp
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

#include "kbrandomshotstrategy.h"

KBRandomShotStrategy::KBRandomShotStrategy(KBStrategy *parent) : KBStrategy(parent)
{
    m_destroyer = new KBDestroyShipStrategy(this);
    m_destroying = false;
}

KBRandomShotStrategy::~KBRandomShotStrategy()
{
    if(m_destroyer != 0)
	delete m_destroyer;
}

void KBRandomShotStrategy::init(KBattleField *field, const QRect &field_rect)
{
    KBStrategy::init(field, field_rect);
    KRandomSequence rand;
    m_column = (int) rand.getLong(m_fieldRect.width() - 1);
    m_row = (int) rand.getLong(m_fieldRect.height() - 1);

    if(m_destroyer != 0)
	m_destroyer->init(field, field_rect);
}

const QPoint KBRandomShotStrategy::nextShot()
{
    if(hasMoreShots())
    {
	if(m_destroying)
	    return m_destroyer->nextShot();
	else if(advance())
	    return QPoint(m_column, m_row);
    }

    return QPoint(0, 0);
}

bool KBRandomShotStrategy::advance()
{
    while(enemyFieldStateAt(m_column, m_row) == SHOT)
    {
	m_column = m_randomSeq.getLong(m_fieldRect.width());
	m_row = m_randomSeq.getLong(m_fieldRect.height());
    }
    return true;
}

bool KBRandomShotStrategy::hasMoreShots()
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

void KBRandomShotStrategy::shotAt(const QPoint &pos)
{
    m_prevShots->append(new QPoint(pos));
}
