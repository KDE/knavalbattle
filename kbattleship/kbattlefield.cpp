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
#include <kship.h>
#include "kbattlefield.moc"

KBattleField::KBattleField(QWidget *parentw, const char *name, bool grid) : KGridWidget(parentw, name, grid)
{
    m_parent_widget = static_cast<QWidget *>(parent());
    m_width = m_parent_widget->width();
    m_canDraw = true;
    
    m_ownfieldx = 8;
    m_ownfieldy = 8;
    m_enemyfieldx = 8;
    m_enemyfieldy = 8;
    
    clearOwnField();
    clearEnemyField();
    drawOwnField();
    drawEnemyField();
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

void KBattleField::drawOwnField()
{
    if(!m_canDraw)
	return;
	
    KBattleshipApp *app = static_cast<KBattleshipApp *>(parent()->parent()->parent()->parent());
    KShip *ship = 0;

    for(int i = 0; i != m_ownfieldx; i++)
    {
        for(int j = 0; j != m_ownfieldy; j++)
        {
            setValues((((i + 1) * gridSize()) + ownXPosition()), (j + 1) * gridSize(), gridSize());
            switch(m_ownfield[i][j])
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
		    ship = app->getXYShip(i, j);
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
		    drawSquare();	
		    ship = app->getXYShip(i, j);
		    if(ship->placedLeft())
		        drawShipIcon(m_ownfield[i][j], true);
		    else
		        drawShipIcon(m_ownfield[i][j]);	
		    break;
	    }
        }		
    }
    
    finished();
}

void KBattleField::drawEnemyField()
{
    if(!m_canDraw)
	return;

    KBattleshipApp *app = static_cast<KBattleshipApp *>(parent()->parent()->parent()->parent());

    for(int i = 0; i != m_enemyfieldx; i++)
    {
        for(int j = 0; j != m_enemyfieldy; j++)
        {
            setValues((((i + 1) * gridSize()) + enemyXPosition()), (j + 1) * gridSize(), gridSize());
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
		    
		case KBattleField::DEATH:
		    drawSquare();	
		    drawDeathIcon();
		    break;	
		    
		default:
		    drawSquare();	
		    KShip *ship = app->getXYShip(i, j);
		    if(ship->placedLeft())
		        drawShipIcon(m_enemyfield[i][j], true);
		    else
		        drawShipIcon(m_enemyfield[i][j]);	
		    break;
	    }
        }		
    }
    
    finished();
}

int KBattleField::ownXPosition()
{
    return 15;
}

int KBattleField::enemyXPosition()
{
    return m_width / 2;
}

int KBattleField::rectX()
{
    return 15;
}

QRect KBattleField::getOwnRect()
{
    return QRect(gridSize() + ownXPosition(), gridSize(), ((m_ownfieldx - 1) * gridSize()) + ownXPosition() + (2 * m_ownfieldy), ((m_ownfieldy - 1) * gridSize()) + gridSize());
}

QRect KBattleField::getEnemyRect()
{
    return QRect(gridSize() + enemyXPosition(), gridSize(), ((m_enemyfieldx - 1) * gridSize()) + rectX() + (2 * m_enemyfieldy), ((m_enemyfieldy - 1) * gridSize()) + gridSize());
}
