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

#include <kalphapainter.h>
#include <qpixmap.h>
#include "kbattlefield.h"
#include "kgridwidget.moc"

KGridWidget::KGridWidget(QWidget *parent, const char *name) : QWidget(parent, name)
{
    doubleBuffer = new QPixmap(parent->width(), parent->height());
    doubleBuffer->fill(QApplication::palette().color(QPalette::Normal, QColorGroup::Background));
    cacheImages();
}

KGridWidget::~KGridWidget()
{
}

void KGridWidget::cacheImages()
{
    seaPng = QImage(findIcon("sea.png"));
    waterPng = QImage(findIcon("water.png"));
    hitPng = QImage(findIcon("hit.png"));
    deathPng = QImage(findIcon("death.png"));
    ship1p1Png = QImage(findIcon("ship1-1.png"));
    ship1p1rPng = QImage(findIcon("ship1-1-r.png"));
    ship2p1Png = QImage(findIcon("ship2-1.png"));
    ship2p1rPng = QImage(findIcon("ship2-1-r.png"));
    ship2p2Png = QImage(findIcon("ship2-2.png"));
    ship2p2rPng = QImage(findIcon("ship2-2-r.png"));
    ship3p1Png = QImage(findIcon("ship3-1.png"));
    ship3p1rPng = QImage(findIcon("ship3-1-r.png"));
    ship3p2Png = QImage(findIcon("ship3-2.png"));
    ship3p2rPng = QImage(findIcon("ship3-2-r.png"));
    ship3p3Png = QImage(findIcon("ship3-3.png"));
    ship3p3rPng = QImage(findIcon("ship3-3-r.png"));
    ship4p1Png = QImage(findIcon("ship4-1.png"));
    ship4p1rPng = QImage(findIcon("ship4-1-r.png"));
    ship4p2Png = QImage(findIcon("ship4-2.png"));
    ship4p2rPng = QImage(findIcon("ship4-2-r.png"));
    ship4p3Png = QImage(findIcon("ship4-3.png"));
    ship4p3rPng = QImage(findIcon("ship4-3-r.png"));
    ship4p4Png = QImage(findIcon("ship4-4.png"));
    ship4p4rPng = QImage(findIcon("ship4-4-r.png"));
}

void KGridWidget::setValues(int x, int y, int size)
{
    internalx = x;
    internaly = y;
    internalSize = size;
}

void KGridWidget::drawSquare()
{
    drawIcon(seaPng);
}

void KGridWidget::drawWaterIcon()
{
    drawIcon(waterPng);
}

void KGridWidget::drawHitIcon()
{
    drawIcon(hitPng);
}

void KGridWidget::drawDeathIcon()
{
    drawIcon(deathPng);
}

void KGridWidget::drawShipIcon(int type, bool rotate, bool hit)
{
    int ship = 0;
    int part = 0;

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

    switch(ship)
    {
	case 1:
	    if(!rotate)
		drawIcon(ship1p1Png, hit);
	    else
		drawIcon(ship1p1rPng, hit);
	    break;
		
	case 2:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship2p1Png, hit);
		    else
			drawIcon(ship2p1rPng, hit);
		    break;
		
		case 2:
		    if(!rotate)
			drawIcon(ship2p2Png, hit);
		    else
			drawIcon(ship2p2rPng, hit);
		    break;
	    }
	    break;
	
	case 3:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship3p1Png, hit);
		    else
			drawIcon(ship3p1rPng, hit);
		    break;
		
		case 2:
		    if(!rotate)
			drawIcon(ship3p2Png, hit);
		    else
			drawIcon(ship3p2rPng, hit);
		    break;
		    
		case 3:
		    if(!rotate)
			drawIcon(ship3p3Png, hit);
		    else
			drawIcon(ship3p3rPng, hit);
		    break;
	    }		    
	    break;
	    
	case 4:
	    switch(part)
	    {
		case 1:
		    if(!rotate)
			drawIcon(ship4p1Png, hit);
		    else
			drawIcon(ship4p1rPng, hit);
		    break;
		
		case 2:
		    if(!rotate)
			drawIcon(ship4p2Png, hit);
		    else
			drawIcon(ship4p2rPng, hit);
		    break;
		    
		case 3:
		    if(!rotate)
			drawIcon(ship4p3Png, hit);
		    else
			drawIcon(ship4p3rPng, hit);
		    break;
		    
		case 4:
		    if(!rotate)
			drawIcon(ship4p4Png, hit);
		    else
			drawIcon(ship4p4rPng, hit);
		    break;
	    }
	    break;	    
    }
}

void KGridWidget::drawIcon(QImage icon, bool hitBlend)
{
    if(!hitBlend)
    {
	QPainter painter;
	painter.begin(doubleBuffer);
	QPixmap *internalPixmap = new QPixmap(internalSize, internalSize);
	internalPixmap->convertFromImage(icon);
	painter.drawPixmap(internalx, internaly, *internalPixmap);
	painter.end();
    }
    else
    {
	// Niko Z:
	// make this work!
	QImage internalBlend = icon;
	QPainter painter;
	painter.begin(doubleBuffer);
	KAlphaPainter::draw(&painter, hitPng, internalBlend, internaly, internaly);
	painter.end();
    }
}

QString KGridWidget::findIcon(const QString &name)
{
    return locate("data", "kbattleship/pictures/" + name);
}

void KGridWidget::finished()
{
    QPainter painter;
    painter.begin(static_cast<QWidget *>(parent()->parent()));
    painter.drawPixmap(0, 0, *doubleBuffer);
    painter.end();
}
