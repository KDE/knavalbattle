/***************************************************************************
                               kgridwidget.cpp
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <wildfox@kde.org>
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

void KGridWidget::drawSquare(QPainter *painter)
{
    if(internalPixmap)
	delete internalPixmap;

    internalPixmap = new QPixmap(internalSize, internalSize);

    QBrush blackBrush(green);

    if(!painter->isActive())
	painter->begin(internalPixmap);

    painter->setBrush(blackBrush);
    painter->drawRect(internalx, internaly, internalSize, internalSize);
    bitBlt(this, internalx, internaly, internalPixmap);
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

void KGridWidget::drawShipIcon(int &ship)
{
    drawIcon("ship-" + QString::number(ship + 1) + ".png");
}

void KGridWidget::drawIcon(const QString &iconName)
{
    KStandardDirs *stdDirs = KGlobal::dirs();
    KImageIO::registerFormats();
    QString picDir;
    QStringList picDirl = stdDirs->findDirs("data", "kbattleship");
    for(QStringList::Iterator it = picDirl.begin(); it != picDirl.end(); it++)
    {
	picDir = *it;
	break;
    }

    picDir = picDir + "pictures/";

    bitBlt(this, internalx, internaly, new QPixmap(picDir + iconName));
}
