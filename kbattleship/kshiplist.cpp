/***************************************************************************
                                kshiplist.cpp
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

#include "kshiplist.moc"

KShipList::KShipList()
{
    shipsadded = 4;
    shiplist.setAutoDelete( true );
}

KShipList::~KShipList()
{
}

bool KShipList::canAddShips()
{
    if( shipsadded >= 0)
	return true;
    else
	return false;
}

void KShipList::addNewShip( int fieldx, int fieldy, int type )
{
    shipsadded--;
    KShipType shiptype;
    shiptype.setType( type );
    shiplist.append( new KShip( fieldx, fieldx + shipCount(), fieldy, fieldy + shipCount(), shiptype ) );
}

int KShipList::shipCount()
{
    return shipsadded;
}
