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

KBattleField::KBattleField(QWidget *parentw, const char *name, int type) : KGridWidget(parentw, name)
{
    m_parent_widget = static_cast<QWidget *>(parent());
    m_type = type;
    m_fieldx = 8;
    m_fieldy = 8;
    
    clearField();
    drawField();
}

KBattleField::~KBattleField()
{
}

void KBattleField::clearField()
{
    for(int i = 0; i != m_fieldx; i++)
    {
        for(int j = 0; j != m_fieldy; j++)
    	{
	    m_field[i][j] = KBattleField::FREE;
	}
    }
}

void KBattleField::drawField()
{
    KBattleshipApp *app = static_cast<KBattleshipApp *>(parent()->parent()->parent()->parent());

    for(int i = 0; i != m_fieldx; i++)
    {
        for(int j = 0; j != m_fieldy; j++)
        {
            setValues((((i + 1) * gridSize()) + xPosition()), (j + 1) * gridSize(), gridSize());
            switch(m_field[i][j])
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
		    if(m_type == KBattleField::OWNFIELD)
		    {
			KShip *ship = app->getXYShip(i, j);
			if(ship->shipxstart() != ship->shipxstop())
			    drawShipIcon(m_field[i][j], true);
			else
			    drawShipIcon(m_field[i][j]);	
			drawHitIcon();
		    }
                    else
			drawHitIcon();
		    break;
		    
		case KBattleField::DEATH:
		    drawSquare();	
		    drawDeathIcon();
		    break;	
		    
		default:
		    drawSquare();	
		    KShip *ship = app->getXYShip(i, j);
		    if(ship->shipxstart() != ship->shipxstop())
		        drawShipIcon(m_field[i][j], true);
		    else
		        drawShipIcon(m_field[i][j]);	
		    break;
	    }
        }		
    }
}

int KBattleField::xPosition()
{
    if(m_type == KBattleField::OWNFIELD)
        return 15;
    else
        return (static_cast<QWidget *>(parent())->width() / 2);
}

int KBattleField::rectX()
{
    return 15;
}

QRect KBattleField::getRect()
{
    if(m_type == KBattleField::OWNFIELD)
	return QRect(gridSize() + xPosition(), gridSize(), ((m_fieldx - 1) * gridSize()) + xPosition() + (2 * m_fieldy), ((m_fieldy - 1) * gridSize()) + gridSize());
    else
    	return QRect(gridSize() + xPosition(), gridSize(), ((m_fieldx - 1) * gridSize()) + rectX() + (2 * m_fieldy), ((m_fieldy - 1) * gridSize()) + gridSize());
}
