/***************************************************************************
                                kstatdlg.cpp
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

#include "kstatdialog.moc"

KStatDialog::KStatDialog(QWidget *parent, const char *name) : statDlg(parent, name)
{
    setMinimumSize(80, 300);
}

KStatDialog::~KStatDialog()
{
}

void KStatDialog::setShot()
{
    setShot(getShot() + 1);
}

void KStatDialog::setShot(int shot)
{
    ShotLCD->display(shot);
}

void KStatDialog::setHit()
{
    setHit(getHit() + 1);
}

void KStatDialog::setHit(int hit)
{
    HitLCD->display(hit);
}

void KStatDialog::setWater()
{
    setWater(getWater() + 1);
}

void KStatDialog::setWater(int water)
{
    WaterLCD->display(water);
}

void KStatDialog::clear()
{
    ShotLCD->display(0);
    HitLCD->display(0);
    WaterLCD->display(0);
}

int KStatDialog::getShot()
{
    return ShotLCD->intValue();
}

int KStatDialog::getHit()
{
    return HitLCD->intValue();    
}

int KStatDialog::getWater()
{
    return WaterLCD->intValue();
}
