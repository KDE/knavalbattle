/***************************************************************************
                                 kmessage.h
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

#ifndef KGRIDWIDGET_H
#define KGRIDWIDGET_H

#include <kdebug.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <kimageio.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbrush.h>
#include <qwidget.h>

class KGridWidget : public QWidget
{
    Q_OBJECT
    public:
        KGridWidget();
        ~KGridWidget();
	
	void setValues( int x, int y, int size );
	void drawSquare( QPainter *painter );
	void drawWaterIcon( QPainter *painter );
	void drawIcon( QPainter *painter, QString iconName );

    private:
        int internalX;
        int internalY;
        int internalSize;
	QPixmap *internalPixmap;
};

#endif
