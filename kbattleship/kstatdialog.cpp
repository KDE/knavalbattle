/***************************************************************************
                                kstatdlg.cpp
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

#include <qlcdnumber.h>

#include <kprogress.h>

#include "kstatdialog.moc"

KStatDialog::KStatDialog(QWidget *parent, const char *name) : statDlg(parent, name)
{
    setMinimumSize(150, 300);
    ownForce->setBarStyle(KProgress::Blocked);
    ownForce->setTextEnabled(false);
    ownForce->setRange(0, 10);
    enemyForce->setBarStyle(KProgress::Blocked);
    enemyForce->setTextEnabled(false);
    enemyForce->setRange(0, 10);
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

void KStatDialog::setOwnForce(int force)
{
    setForce(ownForce, force);
}

void KStatDialog::setEnemyForce(int force)
{
    setForce(enemyForce, force);
}

void KStatDialog::clear()
{
    ShotLCD->display(0);
    HitLCD->display(0);
    WaterLCD->display(0);
    setOwnForce(10);
    setEnemyForce(10);
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
#include <kdebug.h>
void KStatDialog::setForce(KProgress *bar, int value)
{
    kdDebug() << "value: " << value << ", r: " << ((10 - value) * 255 / 10)
              << ", g: " << (value * 255 / 10) << endl;
    bar->setBarColor(QColor((10 - value) * 255 / 10, value * 255 / 10, 0));
    bar->setValue(value);
}

