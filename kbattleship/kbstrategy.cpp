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
	m_viableShots = 0;
}

KBStrategy::~KBStrategy()
{
	m_prevShots->setAutoDelete(true);
	m_prevShots->clear();
	delete m_prevShots;
	if (m_parent == 0 && m_viableShots != 0)
	{
		delete[] m_viableShots;
	}
}

/* Returns the master strategy's shot list. */
QList<QPoint> *KBStrategy::masterShotList()
{
	return (m_parent == 0) ? m_prevShots : m_parent->masterShotList();
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
	if (m_parent == 0)
	{
		if (m_viableShots == 0)
		{
			m_viableShots = new bool[(field_rect.width()*field_rect.height())];
		}
		for (int x = 0; x < field_rect.width(); ++x)
		{
			for (int y = 0; y < field_rect.height(); ++y)
			{
				//m_viableShots[x, y] = true;
				setViablePos(x, y, true);
			}
		}
	}
	else
	{
		m_viableShots = m_parent->getViableShots();
	}
}

/* Returns the field type of position (x, y) on the user player's field */
int KBStrategy::enemyFieldStateAt(int x, int y)
{
	if (!isViablePos(x, y))
		return SHOT; // faking SHOT if position is not possible ship position

	switch(m_battleField->ownState(x, y))
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

bool* KBStrategy::getViableShots()
{
	return m_viableShots;
}

bool KBStrategy::isViablePos(int x, int y)
{
	return m_viableShots[(m_fieldRect.width()*y + x)];
}

void KBStrategy::setViablePos(int x, int y, bool viable)
{
	m_viableShots[(m_fieldRect.width()*y + x)] = viable;
}

