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
    return ownfield->getState(fieldx, fieldy);
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
    int fieldx, fieldy, fieldTopPos, fieldBottomPos, fieldLeftPos, fieldRightPos, i, j;
    fieldx = 0;
    fieldy = 0;
    
    if(event->button() != (LeftButton || RightButton))
	return;

    if(event->x() <= (width() / 2) - 15 && event->x() >= 46)
    {
    	if(event->y() >= 35 && event->y() <= (height() / 2) + 125)
	{
	    fieldTopPos = 35;
	    fieldBottomPos = (height() / 2) + 125;
	    
	    fieldLeftPos = 45;
	    fieldRightPos = (width() / 2) + 125;
	    
	    i = 0;
	    j = 0;
	    
	    for(i = fieldLeftPos; i <= fieldRightPos; i += 30)
	    {
		j++;
		if(event->x() >= i - 30 && event->x() <= i + 30)
		{
		    fieldx = j - 1;
		    break;
		}
	    }

	    i = 0;
	    j = 0;
	
	    for(i = fieldTopPos; i <= fieldBottomPos; i += 30)
	    {
		j++;
		if(event->y() >= i - 30 && event->y() <= i + 30)
		{
		    fieldy = j - 1;
		    break;
		}
	    }
	    emit ownFieldClicked(fieldx, fieldy, event->button());
	}
    }
    else if(event->x() >= (width() / 2) + 15 && event->x() <= width() - 46)
    {
    	if(event->y() >= 35 && event->y() <= (height() / 2) + 125)
	{
	    fieldTopPos = 35;
	    fieldBottomPos = (height() / 2) + 125;
	    
	    fieldLeftPos = 45;
	    fieldRightPos = (width() / 2) + 125;

	    i = 0;
	    j = 0;
	    
	    for(i = fieldLeftPos; i <= fieldRightPos; i += 30)
	    {
		j++;
		if(event->x() >= i + (height() / 2) - 30 && event->x() <= i + (height() / 2) + 45)
		{
		    fieldx = j - 5;
		    break;
		}
	    }
        	
	    i = 0;
	    j = 0;
	
	    for(i = fieldTopPos; i <= fieldBottomPos; i += 30)
	    {
		j++;
		if(event->y() >= i - 30 && event->y() <= i + 30)
		{
		    fieldy = j - 1;
		    break;
		}		
	    }
	    emit enemyFieldClicked(fieldx, fieldy);
	}
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
