/***************************************************************************
                           kbdestroyshipstrategy.cpp
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

#include "kbdestroyshipstrategy.h"

KBDestroyShipStrategy::KBDestroyShipStrategy(KBStrategy *parent) : KBStrategy(parent)
{
	m_working = false;
}

KBDestroyShipStrategy::~KBDestroyShipStrategy()
{
}

void KBDestroyShipStrategy::init(KBattleField *field, const QRect &field_rect)
{
	KBStrategy::init(field, field_rect);
	m_working = false;
}

const QPoint KBDestroyShipStrategy::nextShot()
{
	if(hasMoreShots())
		return QPoint(m_column, m_row);
	else
		return m_start;
}

bool KBDestroyShipStrategy::hasMoreShots()
{
	if(!m_working)
		return false;

	if(shipDestroyed())
	{
		m_working = false;
		markBorderingFields();
		return false;
	}

	if(enemyFieldStateAt(m_column, m_row) != KBStrategy::SHOT)
		return true;

	// last shot was no success :(
	if(m_battleField->ownState(m_column, m_row) == KBattleField::WATER)
	{
		m_column = m_start.x();
		m_row = m_start.y();
	}

	switch(m_direction)
	{
		case VERTICAL:
			if(searchUp() || searchDown())
				return true;
			else
			{
				//kdDebug << "KBDestroyShipStrategy: failed vertical search!" << endl;
				m_working = false;
			}
			break;

		case HORIZONTAL:
			if(searchLeft() || searchRight())
				return true;
			else
			{
				//kdDebug << "KBDestroyShipStrategy: failed horizontal search!" << endl;
				m_working = false;
			}
			break;

		default:
			int up = m_row > 0 ? m_battleField->ownState(m_column, m_row - 1) : -1;
			int down = m_row < (m_fieldRect.height() - 1) ? m_battleField->ownState(m_column, m_row + 1) : -1;
			int left = m_column > 0 ? m_battleField->ownState(m_column - 1, m_row) : -1;
			int right = m_column < (m_fieldRect.width() - 1) ? m_battleField->ownState(m_column + 1, m_row) : -1;

			if((up != -1 && up == KBattleField::HIT) || (down != -1 && down == KBattleField::HIT))
			{
				m_direction = VERTICAL;
				return hasMoreShots();
			}

			if((left != -1 && left == KBattleField::HIT) || (right != -1 && right == KBattleField::HIT))
			{
				m_direction = HORIZONTAL;
				return hasMoreShots();
			}

			if(searchUp() || searchDown() || searchLeft() || searchRight())
				return true;
			else
			{
				//kdDebug << "KBDestroyStrategy: ship unsinkable?" << endl;
				m_working = false;
			}
			break;
	}
	return false;
}

void KBDestroyShipStrategy::shotAt(const QPoint &pos)
{
	m_prevShots->append(new QPoint(pos));
}

void KBDestroyShipStrategy::destroyShipAt(const QPoint pos)
{
	if(enemyFieldStateAt(pos.x(), pos.y()) == FREE || m_battleField->ownState(pos.x(), pos.y()) == KBattleField::DEATH || m_battleField->ownState(pos.x(), pos.y()) == KBattleField::WATER)
		m_working = false;
	else
	{
		m_working = true;
		m_start = pos;
		m_column = pos.x();
		m_row = pos.y();
		m_direction = NODIR;
	}
}

bool KBDestroyShipStrategy::searchUp()
{
	int row = m_row;
	int prevCol = m_column - 1;
	int nextCol = m_column + 1;

	while(row >= 0 && (m_row - row) < 4 && enemyFieldStateAt(m_column, row) == KBStrategy::SHOT)
	{
		if(m_battleField->ownState(m_column, row) == KBattleField::WATER)
			return false;

		row--;

		bool leftOK = true;
		bool rightOK = true;
		if(prevCol >= 0)
			leftOK = (enemyFieldStateAt(prevCol, row) == FREE) || (m_battleField->ownState(prevCol, row) == KBattleField::WATER);

		if(nextCol < m_fieldRect.width())
			rightOK = (enemyFieldStateAt(nextCol, row) == FREE) || (m_battleField->ownState(nextCol, row) == KBattleField::WATER);

		if(!(rightOK && leftOK))
			return false;
	}

	if(row < 0 || (m_row - row) >= 4)
		return false;

	m_row = row;
	return true;
}

bool KBDestroyShipStrategy::searchDown()
{
	int row = m_row;
	int prevCol = m_column - 1;
	int nextCol = m_column + 1;

	while(row < m_fieldRect.height() && (row - m_row) < 4 && enemyFieldStateAt(m_column, row) == KBStrategy::SHOT)
	{
		if(m_battleField->ownState(m_column, row) == KBattleField::WATER)
			return false;

		row++;

		bool leftOK = true;
		bool rightOK = true;
		if(prevCol >= 0)
			leftOK = (enemyFieldStateAt(prevCol, row) == FREE) || (m_battleField->ownState(prevCol, row) == KBattleField::WATER);

		if(nextCol < m_fieldRect.width())
			rightOK = (enemyFieldStateAt(nextCol, row) == FREE) || (m_battleField->ownState(nextCol, row) == KBattleField::WATER);

		if(!(rightOK && leftOK))
			return false;
	}

	if(row >= m_fieldRect.height() || (row - m_row) >= 4)
		return false;

	m_row = row;
	return true;
}

bool KBDestroyShipStrategy::searchLeft()
{
	int col = m_column;
	int prevRow = m_row - 1;
	int nextRow = m_row + 1;

	while(col >= 0 && (m_column - col) < 4 && enemyFieldStateAt(col, m_row) == KBStrategy::SHOT)
	{
		if(m_battleField->ownState(col, m_row) == KBattleField::WATER)
			return false;

		col--;

		bool upOK = true;
		bool downOK = true;
		if(prevRow >= 0)
			upOK = (enemyFieldStateAt(col, prevRow) == FREE) || (m_battleField->ownState(col, prevRow) == KBattleField::WATER);

		if(nextRow < m_fieldRect.height())
			downOK = (enemyFieldStateAt(col, nextRow) == FREE) || (m_battleField->ownState(col, nextRow) == KBattleField::WATER);

		if(!(upOK && downOK))
			return false;
	}

	if(col < 0 || (m_column - col) >= 4)
		return false;

	m_column = col;
	return true;
}

bool KBDestroyShipStrategy::searchRight()
{
	int col = m_column;
	int prevRow = m_row - 1;
	int nextRow = m_row + 1;

	while(col < m_fieldRect.width() && (col - m_column) < 4 && enemyFieldStateAt(col, m_row) == KBStrategy::SHOT)
	{
		if(m_battleField->ownState(col, m_row) == KBattleField::WATER)
			return false;

		col++;

		bool upOK = true;
		bool downOK = true;
		if(prevRow >= 0)
			upOK = (enemyFieldStateAt(col, prevRow) == FREE) || (m_battleField->ownState(col, prevRow) == KBattleField::WATER);

		if(nextRow < m_fieldRect.height())
			downOK = (enemyFieldStateAt(col, nextRow) == FREE) || (m_battleField->ownState(col, nextRow) == KBattleField::WATER);

		if(!(upOK && downOK))
			return false;
	}

	if(col >= m_fieldRect.width() || (col - m_column) >= 4)
		return false;

	m_column = col;
	return true;
}

bool KBDestroyShipStrategy::shipDestroyed()
{
	int col = m_start.x();
	int row = m_start.y();
	int state = m_battleField->ownState(col, row);

	while(m_direction != HORIZONTAL && row >= 0 && state != KBattleField::FREE && state != KBattleField::WATER)
	{
		if(enemyFieldStateAt(col, row) == SHIP)
			return false;

		row--;
		if(row >= 0)
			state = m_battleField->ownState(col, row);
	}

	row = m_start.y();
	state = m_battleField->ownState(col, row);
	while(m_direction != HORIZONTAL && row < m_fieldRect.height() && state != KBattleField::FREE && state != KBattleField::WATER)
	{
		if(enemyFieldStateAt(col, row) == SHIP)
			return false;

		row++;
		if(row < m_fieldRect.height())
			state = m_battleField->ownState(col, row);
	}

	row = m_start.y();
	state = m_battleField->ownState(col, row);
	while(m_direction != VERTICAL && col >= 0 && state != KBattleField::FREE && state != KBattleField::WATER)
	{
		if(enemyFieldStateAt(col, row) == SHIP)
			return false;

		col--;
		if(col >= 0)
			state = m_battleField->ownState(col, row);
	}

	col = m_start.x();
	state = m_battleField->ownState(col, row);
	while(m_direction != VERTICAL && col < m_fieldRect.width() && state != KBattleField::FREE && state != KBattleField::WATER)
	{
		if(enemyFieldStateAt(col, row) == SHIP)
			return false;

		col++;
		if(col < m_fieldRect.width())
			state = m_battleField->ownState(col, row);
	}

	return true;
}

void KBDestroyShipStrategy::markBorderingFields()
{
	int col = m_start.x();
	int row = m_start.y();
	int i,j;

	if (m_direction == VERTICAL)
	{
		while (m_fieldRect.contains(col, row) &&
		       m_battleField->ownState(col, row) == KBattleField::HIT)
		{
			row--;
		}
		if (row >= 0)
		{ // above the ship
			setViablePos(col, row, false);
		}
		row++;
		i = col+1;  // right of the ship
		j = col-1;  // left of the ship
		while (m_fieldRect.contains(col, row) &&
		       m_battleField->ownState(col, row) == KBattleField::HIT)
		{
			if (m_fieldRect.contains(i, row))
				setViablePos(i, row, false);
			if (m_fieldRect.contains(j, row))
				setViablePos(j, row, false);
			setViablePos(col, row, false);
    	row++;
		}
		if (m_fieldRect.contains(col, row))
		{ // below the ship
			setViablePos(col, row, false);
		}
	}
	else if (m_direction == HORIZONTAL)
	{
		while (m_fieldRect.contains(col, row) &&
		       m_battleField->ownState(col, row) == KBattleField::HIT)
		{
			col--;
		}
		if (col >= 0)
		{ // left of the ship
			setViablePos(col, row, false);
		}
		col++;
		i = row+1;  // below the ship
		j = row-1;  // above the ship
		while (m_fieldRect.contains(col, row) &&
		       m_battleField->ownState(col, row) == KBattleField::HIT)
		{
			if (m_fieldRect.contains(col, i))
				setViablePos(col, i, false);
			if (m_fieldRect.contains(col, j))
				setViablePos(col, j, false);
			setViablePos(col, row, false);
    	col++;
		}
		if (m_fieldRect.contains(col, row))
		{ // right of the ship
			setViablePos(col, row, false);
		}
	}
	else
	{
		if (row > 0)
			setViablePos(col, (row-1), false);
		if (row < (m_fieldRect.height()-1))
			setViablePos(col, (row+1), false);
		if (col > 0)
			setViablePos((col-1), row, false);
		if (col < (m_fieldRect.width()-1))
			setViablePos((col+1), row, false);
	}
}
