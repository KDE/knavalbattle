/***************************************************************************
                                    kshiplist.h
                                  -----------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
                (c) 2001 Kevin Krammer <kevin.krammer@gmx.at>

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
#include <qptrlist.h>
#include "kbattlefield.h"
#include "kship.h"

class KShipList : public QObject
{
	Q_OBJECT
public:
	KShipList();

	KShip *shipAt(int x, int y);
	int shipTypeAt(int x, int y);

	void clear();

	void addNewShip(int button, int fieldx, int fieldy);
	bool addNewShip(bool vertical, int fieldx, int fieldy);

	bool canAddShips();

	int shipCount() { return m_shipsadded; }

	int m_fieldx;
	int m_fieldy;

signals:
	void sigLastShipAdded();
	void sigOwnFieldDataChanged(int, int, int);

private:
	QPtrList<KShip> m_shiplist;
	int m_shipsadded;
};

#endif
