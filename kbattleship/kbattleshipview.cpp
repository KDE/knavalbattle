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
#include <kdebug.h>
#include "kbattleship.h"
#include "kbattleshipview.moc"

KBattleshipView::KBattleshipView(QWidget *parent, const char *name, bool draw) : QWidget(parent, name), m_drawGrid(draw)
{
    setMinimumSize(600, 300);
    setMouseTracking(true);
    installEventFilter(this);
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

int KBattleshipView::getOwnFieldState(int fieldx, int fieldy)
{
    return battlefield->getOwnState(fieldx, fieldy);
}

int KBattleshipView::getEnemyFieldState(int &fieldx, int &fieldy)
{
    return battlefield->getEnemyState(fieldx, fieldy);
}

void KBattleshipView::previewShip(int fieldx, int fieldy, int type, bool rotate)
{
    battlefield->changePreviewData(fieldx, fieldy, type, rotate);
}

void KBattleshipView::changeOwnFieldData(int fieldx, int fieldy, int type)
{
    battlefield->changeOwnData(fieldx, fieldy, type);
    battlefield->drawOwnField();
}

void KBattleshipView::changeEnemyFieldData(int fieldx, int fieldy, int type)
{
    battlefield->changeEnemyData(fieldx, fieldy, type);
    battlefield->drawEnemyField();
}

bool KBattleshipView::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonRelease)
    {
	QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
	
	if(mouseEvent->button() != LeftButton)
	    return false;

	QPoint point(mouseEvent->x(), mouseEvent->y());
	QRect ownRect = battlefield->getOwnRect();
	QRect enemyRect = battlefield->getEnemyRect();
    
	QRect newRect;
    
	int fieldx = 0;
	int fieldy = 0;
    
    	if(ownRect.contains(point))
	    newRect = ownRect;
	else if(enemyRect.contains(point))
	    newRect = enemyRect;
	else
	    return false;
	
	int j = -1;
	
	for(int i = newRect.left(); i <= newRect.right(); i += battlefield->gridSize())
	{
	    j++;
	    QRect tempRect(i, newRect.top(), battlefield->gridSize(), newRect.bottom() - newRect.top());

	    if(tempRect.contains(point))
	    {
		fieldx = j;
		break;
	    }	        
	}

	j = -1;
	
	for(int i = newRect.top(); i <= newRect.bottom(); i += battlefield->gridSize())
	{
	    j++;
	    QRect tempRect(newRect.left(), i, newRect.right() - newRect.left(), battlefield->gridSize());

	    if(tempRect.contains(point))
	    {
		fieldy = j;
		break;
	    }	        
	}
	
	if((mouseEvent->state() & ShiftButton) == 0 && newRect == ownRect)
	    emit ownFieldClicked(fieldx, fieldy, true);
	else if((mouseEvent->state() & ShiftButton) != 0 && newRect == ownRect)
	    emit ownFieldClicked(fieldx, fieldy, false);
	else if(newRect == enemyRect)
	    emit enemyFieldClicked(fieldx, fieldy);
	    
	return true;
    }
    else if(event->type() == QEvent::MouseMove)
    {
	QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
	
	QPoint point(mouseEvent->x(), mouseEvent->y());
	QRect ownRect = battlefield->getOwnRect();

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
	
	    emit mouseOverField(fieldx, fieldy, mouseEvent->state() & ShiftButton);
	}
	else
	    battlefield->drawOwnField();
	
	return true;
    }
    else if(event->type() == QEvent::Paint)
    { 
	battlefield->drawOwnField();
	battlefield->drawEnemyField();
	return true;
    }

    return QWidget::eventFilter(object, event);
}
