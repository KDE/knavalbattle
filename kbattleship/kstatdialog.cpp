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

KStatDialog::KStatDialog( QWidget *parent, const char *name ) : statDlg( parent, name )
{
    setMinimumSize( 110, 200 );
}

KStatDialog::~KStatDialog()
{
}

void KStatDialog::setShot()
{
    ShotLCD->display( ShotLCD->intValue() + 1 );
}

void KStatDialog::setHit()
{
    HitLCD->display( HitLCD->intValue() + 1 );
}

void KStatDialog::setWater()
{
    WaterLCD->display( WaterLCD->intValue() + 1 );
}

void KStatDialog::setScore( int score )
{
    ScoreLCD->display( ScoreLCD->intValue() + score );
}
