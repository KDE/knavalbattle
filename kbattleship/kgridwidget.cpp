/***************************************************************************
                               kgridwidget.cpp
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

#include "kgridwidget.moc"
#include "kbattlefield.h"

KGridWidget::KGridWidget(QWidget *parent, const char *name) : QWidget(parent, name)
{
    internalPixmap = 0;
}

KGridWidget::~KGridWidget()
{
}

void KGridWidget::setValues(int x, int y, int size)
{
    internalx = x;
    internaly = y;
    internalSize = size;
}

void KGridWidget::drawSquare()
{
    QPainter *painter = new QPainter(static_cast<QWidget *>(parent()->parent()));
    
    if(internalPixmap)
	delete internalPixmap;

    internalPixmap = new QPixmap(internalSize, internalSize);

    QBrush blackBrush(blue);

    if(!painter->isActive())
	painter->begin(internalPixmap);

    painter->setBrush(blackBrush);
    painter->drawRect(internalx, internaly, internalSize, internalSize);
    painter->end();
}

void KGridWidget::drawHitIcon()
{
    drawIcon("hit.png");
}

void KGridWidget::drawWaterIcon()
{
    drawIcon("water.png");
}

void KGridWidget::drawDeathIcon()
{
    drawIcon("death.png");
}

void KGridWidget::drawShipIcon(int type, bool rotate)
{
    int ship;
    int part;

    switch(type)
    {
	case KBattleField::SHIP1P1:
	    ship = 1;
	    part = 1;
	    break;
	
	case KBattleField::SHIP2P1:
	    ship = 2;
	    if(!rotate)
		part = 1;
	    else
		part = 2;
	    break;

	case KBattleField::SHIP2P2:
	    ship = 2;
	    if(!rotate)
		part = 2;
	    else
		part = 1;
	    break;
	    
	case KBattleField::SHIP3P1:
	    ship = 3;
	    if(!rotate)
		part = 1;
	    else
		part = 3;
	    break;
	    
	case KBattleField::SHIP3P2:
	    ship = 3;
	    if(!rotate)
		part = 2;
	    else
		part = 2;
	    break;
	    
	case KBattleField::SHIP3P3:
	    ship = 3;
	    if(!rotate)
		part = 3;
	    else
		part = 1;
	    break;
	    
	case KBattleField::SHIP4P1:
	    ship = 4;
	    if(!rotate)
		part = 1;
	    else
		part = 4;
	    break;
	    
	case KBattleField::SHIP4P2:
	    ship = 4;
	    if(!rotate)
		part = 2;
	    else
		part = 3;
	    break;
	    
	case KBattleField::SHIP4P3:
	    ship = 4;
	    if(!rotate)
		part = 3;
	    else
		part = 2;
	    break;

	case KBattleField::SHIP4P4:
	    ship = 4;
	    if(!rotate)
		part = 4;
	    else
		part = 1;
	    break;
	    
    }

    if(!rotate)
	drawIcon("ship" + QString::number(ship) + "-" + QString::number(part) + ".png");
    else
	drawIcon("ship" + QString::number(ship) + "-" + QString::number(part) + "-r.png");
}

void KGridWidget::drawIcon(const QString &iconName)
{
    QString picture = locate("data", "kbattleship/pictures/" + iconName);
    if(picture.isEmpty())
	return;

    QPainter *painter = new QPainter(static_cast<QWidget *>(parent()->parent()));
    
    if(!painter->isActive())
	painter->begin(internalPixmap);

    painter->drawPixmap(internalx, internaly, *(new QPixmap(picture)));
    painter->end();
}
