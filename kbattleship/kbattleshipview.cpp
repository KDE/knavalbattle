/***************************************************************************
                             kbattleshipview.cpp
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

#include <klocale.h>
#include "kbattleshipview.moc"

KBattleshipView::KBattleshipView(QWidget *parent, const char *name, bool draw) : QWidget(parent, name), m_drawGrid(draw)
{
    setMinimumSize(600, 300);
}

KBattleshipView::~KBattleshipView()
{
}

void KBattleshipView::startDrawing()
{
    battlefield = new KBattleField(this, "battlefield", m_drawGrid);
}

void KBattleshipView::clearField()
{
    battlefield->clearOwnField();
    battlefield->clearEnemyField();
}

int KBattleshipView::getEnemyFieldState(int &fieldx, int &fieldy)
{
    return battlefield->getEnemyState(fieldx, fieldy);
}

void KBattleshipView::changeOwnFieldData(int fieldx, int fieldy, int type)
{
    battlefield->changeOwnData(fieldx, fieldy, type);
    paintOwnField();
}

void KBattleshipView::changeEnemyFieldData(int fieldx, int fieldy, int type)
{
    battlefield->changeEnemyData(fieldx, fieldy, type);
    paintEnemyField();
}

void KBattleshipView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() != LeftButton && event->button() != RightButton)
	return;

    QPoint point(event->x(), event->y());
    QRect ownRect = battlefield->getOwnRect();
    QRect enemyRect = battlefield->getEnemyRect();
    
    int fieldx = 0;
    int fieldy = 0;
    
    if(ownRect.contains(point))
    {
	int j = -1;
	
	for(int i = ownRect.left(); i <= ownRect.right(); i += battlefield->gridSize())
	{
	    j++;
	    QRect tempRect(i, ownRect.top(), battlefield->gridSize(), ownRect.bottom() - ownRect.top());

	    if(tempRect.contains(point))
	    {
		fieldx = j;
		break;
	    }	        
	}

	j = -1;
	
	for(int i = ownRect.top(); i <= ownRect.bottom(); i += battlefield->gridSize())
	{
	    j++;
	    QRect tempRect(ownRect.left(), i, ownRect.right() - ownRect.left(), battlefield->gridSize());

	    if(tempRect.contains(point))
	    {
		fieldy = j;
		break;
	    }	        
	}
	
	emit ownFieldClicked(fieldx, fieldy, event->button());
    }
    else if(enemyRect.contains(point))
    {
	int j = -1;
	
	for(int i = enemyRect.left(); i <= enemyRect.right(); i += battlefield->gridSize())
	{
	    j++;
	    QRect tempRect(i, enemyRect.top(), battlefield->gridSize(), enemyRect.bottom() - enemyRect.top());

	    if(tempRect.contains(point))
	    {
		fieldx = j;
		break;
	    }	        
	}

	j = -1;
	
	for(int i = enemyRect.top(); i <= enemyRect.bottom(); i += battlefield->gridSize())
	{
	    j++;
	    QRect tempRect(enemyRect.left(), i, enemyRect.right() - enemyRect.left(), battlefield->gridSize());

	    if(tempRect.contains(point))
	    {
		fieldy = j;
		break;
	    }	        
	}
	
	emit enemyFieldClicked(fieldx, fieldy);
    }
}

void KBattleshipView::paintOwnField()
{
    battlefield->drawOwnField();
}

void KBattleshipView::paintEnemyField()
{
    battlefield->drawEnemyField();
}

void KBattleshipView::paintEvent(QPaintEvent *)
{
    battlefield->drawOwnField();
    battlefield->drawEnemyField();
}
