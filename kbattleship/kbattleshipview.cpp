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

KBattleshipView::KBattleshipView(QWidget *parent, const char *name) : QWidget(parent, name)
{
    setMinimumSize(600, 300);
}

KBattleshipView::~KBattleshipView()
{
}

void KBattleshipView::startDrawing()
{
    ownfield = new KBattleField(this, "ownfield", KBattleField::OWNFIELD);
    enemyfield = new KBattleField(this, "enemyfield", KBattleField::ENEMYFIELD);

    toolTip = new KBattleshipViewToolTip(this, ownfield->getRect(), enemyfield->getRect());
    connect(toolTip, SIGNAL(activateTimer()), this, SLOT(toolTipTimeout()));
}

void KBattleshipView::toolTipTimeout()
{
    QTimer::singleShot(3000, toolTip, SLOT(slotTipTimeout()));
}

void KBattleshipView::clearField()
{
    ownfield->clearField();
    enemyfield->clearField();
    paintOwnField();
    paintEnemyField();
}

int KBattleshipView::getEnemyFieldState(int &fieldx, int &fieldy)
{
    return enemyfield->getState(fieldx, fieldy);
}

void KBattleshipView::changeOwnFieldData(int fieldx, int fieldy, int type)
{
    ownfield->changeData(fieldx, fieldy, type);
    paintOwnField();
}

void KBattleshipView::changeEnemyFieldData(int fieldx, int fieldy, int type)
{
    enemyfield->changeData(fieldx, fieldy, type);
    paintEnemyField();
}

void KBattleshipView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() != LeftButton && event->button() != RightButton)
	return;

    QPoint point(event->x(), event->y());
    QRect ownRect = ownfield->getRect();
    QRect enemyRect = enemyfield->getRect();
    
    int fieldx = 0;
    int fieldy = 0;
    
    if(ownRect.contains(point))
    {
	int j = -1;
	
	for(int i = ownRect.left(); i <= ownRect.right(); i += ownfield->gridSize())
	{
	    j++;
	    QRect tempRect(i, ownRect.top(), ownfield->gridSize(), ownRect.bottom() - ownRect.top());

	    if(tempRect.contains(point))
	    {
		fieldx = j;
		break;
	    }	        
	}

	j = -1;
	
	for(int i = ownRect.top(); i <= ownRect.bottom(); i += ownfield->gridSize())
	{
	    j++;
	    QRect tempRect(ownRect.left(), i, ownRect.right() - ownRect.left(), ownfield->gridSize());

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
	
	for(int i = enemyRect.left(); i <= enemyRect.right(); i += ownfield->gridSize())
	{
	    j++;
	    QRect tempRect(i, enemyRect.top(), ownfield->gridSize(), enemyRect.bottom() - enemyRect.top());

	    if(tempRect.contains(point))
	    {
		fieldx = j;
		break;
	    }	        
	}

	j = -1;
	
	for(int i = enemyRect.top(); i <= enemyRect.bottom(); i += ownfield->gridSize())
	{
	    j++;
	    QRect tempRect(enemyRect.left(), i, enemyRect.right() - enemyRect.left(), ownfield->gridSize());

	    if(tempRect.contains(point))
	    {
		fieldy = j;
		break;
	    }	        
	}
	
	emit enemyFieldClicked(fieldx, fieldy);
    }
}

void KBattleshipView::paintEnemyField()
{
    enemyfield->drawField();
}

void KBattleshipView::paintOwnField()
{
    ownfield->drawField();
}

void KBattleshipView::paintEvent(QPaintEvent *)
{
    ownfield->drawField();
    enemyfield->drawField();
}
