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

#include <kstandarddirs.h>
#include <qlcdnumber.h>
#include <QLabel>
#include "kstatdialog.moc"

KStatDialog::KStatDialog(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	pShots->setPixmap(QPixmap(KStandardDirs::locate("data","kbattleship/pictures/stat-shots.png")));
	pHits->setPixmap(QPixmap(locate("data","kbattleship/pictures/stat-hits.png")));
	pWater->setPixmap(QPixmap(locate("data","kbattleship/pictures/stat-water.png")));
}

void KStatDialog::slotAddOwnWon()
{
	OwnLabel->setText(QString::number(OwnLabel->text().toInt() + 1));
}

void KStatDialog::slotAddEnemyWon()
{
	EnemyLabel->setText(QString::number(EnemyLabel->text().toInt() + 1));
}

void KStatDialog::setShot()
{
	setShot(shot() + 1);
}

void KStatDialog::setShot(int shot)
{
	ShotLCD->display(shot);
}

void KStatDialog::setHit()
{
	setHit(hit() + 1);
}

void KStatDialog::setHit(int hit)
{
	HitLCD->display(hit);
}

void KStatDialog::setWater()
{
	setWater(water() + 1);
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

void KStatDialog::clearWon()
{
	OwnLabel->setText(QString::number(0));
	EnemyLabel->setText(QString::number(0));
}

int KStatDialog::shot()
{
	return ShotLCD->intValue();
}

int KStatDialog::hit()
{
	return HitLCD->intValue();
}

int KStatDialog::water()
{
	return WaterLCD->intValue();
}
