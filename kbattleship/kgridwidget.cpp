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

KGridWidget::KGridWidget( QWidget *parent ) : QWidget()
{
    internalParent = parent;
}

KGridWidget::~KGridWidget()
{
}

void KGridWidget::setValues( int &x, int &y, int size )
{
    internalX = x;
    internalY = y;
    internalSize = size;
}

void KGridWidget::drawSquare( QWidget *parent )
{
    QPainter painter( internalParent );
    painter.drawRect( internalX, internalY, internalSize, internalSize );
}
