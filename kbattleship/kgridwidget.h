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
#include <qpainter.h>
#include <qwidget.h>

class KGridWidget : QWidget()
{
    Q_OBJECT
    public:
        KGridWidget( int x, int y, int size, QPainter *painter );
        ~KGridWidget();

    private:
        int internalX;
        int internalY;
        int internalSize;
        QPainter internalPainter;
};

#endif
