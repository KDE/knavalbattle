/***************************************************************************
                           kbdiagonalshotstrategy.cpp
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

#include "kbdiagonalshotstrategy.h"

KBDiagonalShotStrategy::KBDiagonalShotStrategy(KBStrategy *parent) : KBStrategy(parent)
{
	m_column = 0;
	m_row = 0;
	m_vertical = 0;
	m_horizontal = 0;
}

KBDiagonalShotStrategy::~KBDiagonalShotStrategy()
{
}

const QPoint KBDiagonalShotStrategy::nextShot()
{
	if(hasMoreShots())
		return QPoint(m_column, m_row);

	return QPoint(0,0);
}

bool KBDiagonalShotStrategy::advance()
{
	while (m_fieldRect.contains(m_column, m_row))
	{
		if(enemyFieldStateAt(m_column, m_row) != SHOT)
			return true;
		m_column += m_horizontal;
		m_row += m_vertical;
	}

	return false;
}

bool KBDiagonalShotStrategy::hasMoreShots()
{
	return advance();
}

void KBDiagonalShotStrategy::shotAt(const QPoint &pos)
{
	m_prevShots->append(new QPoint(pos));
}

void KBDiagonalShotStrategy::startAt(int col, int row, Direction dir)
{
	m_column = col;
	m_row = row;

	switch(dir)
	{
		case LEFTUP:
			m_vertical = -1;
			m_horizontal = -1;
			break;

		case LEFTDOWN:
			m_vertical = 1;
			m_horizontal = -1;
			break;

		case RIGHTUP:
			m_vertical = -1;
			m_horizontal = 1;
			break;

		case RIGHTDOWN:
			m_vertical = 1;
			m_horizontal = 1;
			break;

		default:
			m_vertical = 0;
			m_horizontal = 0;
			break;
	}
}

QPoint KBDiagonalShotStrategy::endPoint()
{
	int row = m_row;
	int col = m_column;

	if(m_vertical == 0 || m_horizontal == 0)
		return QPoint(col, row);

	while(m_fieldRect.contains(col, row))
	{
		row += m_vertical;
		col += m_horizontal;
	}

	row -= m_vertical;
	col -= m_horizontal;

	return QPoint(col, row);
}
