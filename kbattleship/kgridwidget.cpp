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

KGridWidget::KGridWidget( QWidget *parent, int x, int y, int size ) : QWidget()
{
    internalX = x;
    internalY = y;
    internalSize = size;
    drawSquare( parent );
}

KGridWidget::~KGridWidget()
{
}

void KGridWidget::drawSquare( QWidget *parent )
{
    QPainter painter( parent );
    painter.drawRect( internalX, internalY, internalSize, internalSize );
}
