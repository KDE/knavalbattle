/***************************************************************************
                                  kship.cpp
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

#include "kship.moc"

KShip::KShip(int shipxstart, int shipxstop, int shipystart, int shipystop, int shiplength) : QObject()
{
    internalshipxstart = shipxstart;
    internalshipxstop = shipxstop;
    internalshipystart = shipystart;
    internalshipystop = shipystop;
    internalshiptype = shiplength;
}

KShip::~KShip()
{
}

int KShip::shipxstart()
{
    return internalshipxstart;
}

int KShip::shipxstop()
{
    return internalshipxstop;
}

int KShip::shipystart()
{
    return internalshipystart;
}

int KShip::shipystop()
{
    return internalshipystop;
}

int KShip::shiptype()
{	
    return internalshiptype;
}
