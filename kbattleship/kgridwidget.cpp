/***************************************************************************
                               kgridwidget.cpp
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <nikoz@gymnasium-kerpen.de>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

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

KGridWidget::KGridWidget() : QWidget()
{
}

KGridWidget::~KGridWidget()
{
}

void KGridWidget::setValues( int x, int y, int size )
{
    internalX = x;
    internalY = y;
    internalSize = size;
}

void KGridWidget::drawSquare( QPainter *painter )
{
    if( internalPixmap )
	delete internalPixmap;

    internalPixmap = new QPixmap( internalSize, internalSize );
    
    QBrush blackBrush( green );
        
    painter->begin( internalPixmap );
    painter->setBrush( blackBrush );
    painter->drawRect( internalX, internalY, internalSize, internalSize );
    bitBlt( this, 0, 0, internalPixmap );
}
