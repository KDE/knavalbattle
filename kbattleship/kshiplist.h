/***************************************************************************
                                    kshiplist.h
                                  -----------------
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
 
#ifndef KSHIPLIST_H
#define KSHIPLIST_H

#include <kdebug.h>
#include <qlist.h>
#include "kship.h"
#include "kshiptype.h"

class KShipList : public QObject
{
    Q_OBJECT
    public:
        KShipList();
        ~KShipList();

	void addNewShip( int fieldx, int fieldy, int type );
	bool canAddShips();
	int shipCount();
	    
    private:
	int shipsadded;
	QList<KShip> shiplist;
};

#endif

