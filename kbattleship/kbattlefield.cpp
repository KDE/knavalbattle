/***************************************************************************
                               kbattlefield.cpp
                             -------------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kbattleship.h"
#include "kship.h"
#include "kbattlefield.moc"

KBattleField::KBattleField(QWidget *parent, const char *name, bool grid) : KGridWidget(parent, name, grid)
{
	m_width = parent->width();
	m_canDraw = true;

	m_ownfieldx = 10;
	m_ownfieldy = 10;
	m_enemyfieldx = 10;
	m_enemyfieldy = 10;

	clearOwnField();
	clearEnemyField();
	clearPreviewField();
	drawField();
}

KBattleField::~KBattleField()
{
}

void KBattleField::clearOwnField()
{
	for(int i = 0; i != m_ownfieldx; i++)
	{
		for(int j = 0; j != m_ownfieldy; j++)
		{
			m_ownfield[i][j] = KBattleField::FREE;
		}
	}
}

void KBattleField::clearEnemyField()
{
	for(int i = 0; i != m_enemyfieldx; i++)
	{
		for(int j = 0; j != m_enemyfieldy; j++)
		{
			m_enemyfield[i][j] = KBattleField::FREE;
		}
	}
}

void KBattleField::clearPreviewField()
{
	for(int i = 0; i != m_ownfieldx; i++)
	{
		for(int j = 0; j != m_ownfieldy; j++)
		{
			m_newfield[i][j] = KBattleField::FREE;
	    	m_newdata[i][j] = false;
		}
	}
}

void KBattleField::setPreviewState(int fieldx, int &fieldy, int type, bool rotate)
{
	m_newfield[fieldx][fieldy] = type;
	m_newdata[fieldx][fieldy] = true;
	m_rotatedata[fieldx][fieldy] = rotate;
}

void KBattleField::drawField()
{
	drawOwnField();
	drawEnemyField();
	clearPreviewField();
	finished();
}

void KBattleField::drawOwnField()
{
	if(!m_canDraw)
		return;

	KBattleshipApp *app = static_cast<KBattleshipApp *>(parent()->parent()->parent());
	KShip *ship = 0;
	int data;

	for(int i = 0; i != m_ownfieldx; i++)
	{
		for(int j = 0; j != m_ownfieldy; j++)
		{
			setValues(((i * gridSize()) + ownXPosition()), ((j * gridSize()) + ownYPosition()), gridSize());
			if(!m_newdata[i][j])
				data = m_ownfield[i][j];
			else
				data = m_newfield[i][j];
			switch(data)
			{
				case KBattleField::FREE:
					drawSquare();
					break;

				case KBattleField::WATER:
					drawSquare();
					drawWaterIcon();
					break;

				case KBattleField::HIT:
					drawSquare();
					ship = app->shipAt(i, j);
					if(ship->placedLeft())
						drawShipIcon((ship->shiptype() + 1), (ship->shipxstop() - i + 1), true, true);
					else
						drawShipIcon((ship->shiptype() + 1), (j - ship->shipystart() + 1), false, true);
					break;

				case KBattleField::DEATH:
					drawSquare();
					drawDeathIcon();
					break;

				default:
					ship = app->shipAt(i, j);
					if(ship)
					{
						drawSquare();
						if(m_newdata[i][j])
							data = m_ownfield[i][j];
						drawShipIcon(data, ship->placedLeft());
					}
					else if(!ship)
						drawShipIcon(data, !m_rotatedata[i][j], false, true);
					break;
			}
		}		
	}
}

void KBattleField::drawEnemyField()
{
	if(!m_canDraw)
		return;

	KBattleshipApp *app = static_cast<KBattleshipApp *>(parent()->parent()->parent());

	for(int i = 0; i != m_enemyfieldx; i++)
	{
		for(int j = 0; j != m_enemyfieldy; j++)
		{
			setValues(((i * gridSize()) + enemyXPosition()), ((j * gridSize()) + enemyYPosition()), gridSize());
			switch(m_enemyfield[i][j])
			{
				case KBattleField::FREE:
					drawSquare();
					break;

				case KBattleField::WATER:
					drawSquare();
					drawWaterIcon();
					break;

				case KBattleField::HIT:
					drawSquare();
					drawHitIcon();
					break;

				case KBattleField::BORDER:
					drawSquare();
					drawDeathBorder();
					break;
					
				case KBattleField::DEATH:
					drawSquare();
					drawDeathIcon();
					break;

				default:
					drawSquare();
					KShip *ship = app->shipAt(i, j);
					if(ship->placedLeft())
						drawShipIcon(m_enemyfield[i][j], true);
					else
						drawShipIcon(m_enemyfield[i][j]);
					break;
			}
		}
	}
}

int KBattleField::ownXPosition()
{
	return 22;
}

int KBattleField::ownYPosition()
{
	return 22;
}

int KBattleField::enemyXPosition()
{
	return (m_width / 2) + 10;
}

int KBattleField::enemyYPosition()
{
	return 22;
}

int KBattleField::rectX()
{
	return 22;
}

QRect KBattleField::ownRect()
{
	return QRect(ownXPosition(), ownYPosition(), m_ownfieldx * gridSize(), m_ownfieldy * gridSize());
}

QRect KBattleField::enemyRect()
{
	return QRect(enemyXPosition(), enemyYPosition(), m_enemyfieldx * gridSize(), m_enemyfieldy * gridSize());
}
