/***************************************************************************
                                 kbstrategy.cpp
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

#include "kbstrategy.h"

KBStrategy::KBStrategy(KBStrategy *parent)
{
    m_parent = parent;
    m_prevShots = new QList<QPoint>;
}

KBStrategy::~KBStrategy()
{
    m_prevShots->setAutoDelete(true);
    m_prevShots->clear();
    delete m_prevShots;
}

/* Returns the master strategy's shot list. */
QList<QPoint> *KBStrategy::getMasterShotList()
{
    return (m_parent == 0) ? m_prevShots : m_parent->getMasterShotList();
}

/* the AI player decided to shoot at pos */
void KBStrategy::shotAt(const QPoint &pos)
{
    m_prevShots->append(new QPoint(pos));
}

void KBStrategy::init(KBattleField *field, const QRect &field_rect)
{
    m_battleField = field;
    m_fieldRect = field_rect;
}

/* Returns the field type of position (x, y) on the user player's field */
int KBStrategy::enemyFieldStateAt(int x, int y)
{
    switch(m_battleField->getOwnState(x, y))
    {
	case KBattleField::FREE:
	    return FREE;
	case KBattleField::WATER:
	case KBattleField::HIT:
	case KBattleField::DEATH:
	    return SHOT;
	default:
	    return SHIP;		
    }	
}
