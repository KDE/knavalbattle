/***************************************************************************
                                    kshiplist.h
                                  -----------------
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
 
#ifndef KSHIPLIST_H
#define KSHIPLIST_H

#include <kmessagebox.h>
#include <klocale.h>
#include <qlist.h>
#include "kbattlefield.h"
#include "kship.h"

class KShipList : public QObject
{
    Q_OBJECT
    public:
        KShipList();
        ~KShipList();

        int getXYShipType(int x, int y);
        KShip *getXYShip(int x, int y);

	void clear();

        void addNewShip(int button, int fieldx, int fieldy);

        void addShip(int fieldx1, int fieldx2, int fieldy1, int fieldy2, int ship);

        void addShip1(int fieldx1, int fieldx2, int fieldy1, int fieldy2);
        void addShip2(int fieldx1, int fieldx2, int fieldy1, int fieldy2);
        void addShip3(int fieldx1, int fieldx2, int fieldy1, int fieldy2);
        void addShip4(int fieldx1, int fieldx2, int fieldy1, int fieldy2);

        bool canAddShips();

        int shipCount() { return shipsadded; }

        KShip *returnIterator(int ship);

        int returnX1Ship(int ship);
        int returnX2Ship(int ship);
        int returnY1Ship(int ship);
        int returnY2Ship(int ship);

    signals:
        void lastShipAdded();
        void ownFieldDataChanged(int, int, int);

    private:
        QList<KShip> shiplist;
        int shipsadded;
        void controlOwnFieldData(int fieldx, int fieldy, int type);
        void placeShipLMB(int fieldx, int fieldy);
        void placeShipRMB(int fieldx, int fieldy);
        void decideShipPlacing(int button, int fieldx, int fieldy);
};

#endif
