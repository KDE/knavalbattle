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

KShip::KShip(int _shipxstart, int _shipxstop, int _shipystart, int _shipystop,
int _shiplength) : QObject()
{
    internalshipxstart = _shipxstart;
    internalshipxstop = _shipxstop;
    internalshipystart = _shipystart;
    internalshipystop = _shipystop;
    internalshiptype = _shiplength;
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
