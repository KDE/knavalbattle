/***************************************************************************
                                kgridwidget.h
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

#ifndef KGRIDWIDGET_H
#define KGRIDWIDGET_H

#include <kdebug.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <kimageio.h>
#include <qpainter.h>
#include <qimage.h>
#include <qbrush.h>
#include <qwidget.h>

class KGridWidget : public QWidget
{
    Q_OBJECT
    public:
        KGridWidget(QWidget *parent, const char *name);
        ~KGridWidget();

    protected:    
        void setValues(int x, int y, int size);
        void drawSquare();
        void drawWaterIcon();
	void drawDeathIcon();
	void drawHitIcon();
        void drawShipIcon(int type, bool rotate = false, bool hitBlend = false);

    private:
	void cacheImages();
        void drawIcon(const QImage &icon, bool hitBlend = false);
	QString findIcon(const QString &name);

        int internalx, internaly, internalSize;
	QImage seaPng, waterPng, hitPng, deathPng;
	QImage ship1p1Png, ship1p1rPng;
	QImage ship2p1Png, ship2p1rPng;
	QImage ship2p2Png, ship2p2rPng;
	QImage ship3p1Png, ship3p1rPng;
	QImage ship3p2Png, ship3p2rPng;
	QImage ship3p3Png, ship3p3rPng;
	QImage ship4p1Png, ship4p1rPng;
	QImage ship4p2Png, ship4p2rPng;
	QImage ship4p3Png, ship4p3rPng;
	QImage ship4p4Png, ship4p4rPng;
};

#endif
