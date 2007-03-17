/***************************************************************************
                               kshiplist.cpp
                            -------------------
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

#include "kshiplist.moc"
#include <kdebug.h>

KShipList::KShipList() : QObject()
{
    //m_shiplist.setAutoDelete(true);
    m_shipsadded = 4;

    m_fieldx = 10;
    m_fieldy = 10;
}

void KShipList::clear()
{
    m_shipsadded = 4;
    qDeleteAll(m_shiplist);
    m_shiplist.clear();
}

int KShipList::shipTypeAt(int x, int y)
{
    int tempx, tempy;
    KShip *shipiterator;
    QListIterator<KShip *> i(m_shiplist);
    while (i.hasNext()) {
        shipiterator = i.next();
        if (shipiterator->shipystart() != shipiterator->shipystop()) {
            for (tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++) {
                if (tempy == y) {
                    if (shipiterator->shipxstart() != shipiterator->shipxstop()) {
                        for (tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++) {
                            if (tempx == x)
                                return shipiterator->shiptype();
                        }
                    } else {
                        tempx = shipiterator->shipxstart();
                        if (tempx == x)
                            return shipiterator->shiptype();
                    }
                }
            }
        } else {
            tempy = shipiterator->shipystart();
            if (tempy == y) {
                if (shipiterator->shipxstart() != shipiterator->shipxstop()) {
                    for (tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++) {
                        if (tempx == x)
                            return shipiterator->shiptype();
                    }
                } else {
                    tempx = shipiterator->shipxstart();
                    if (tempx == x)
                        return shipiterator->shiptype();
                }
            }
        }
    }
    return 99;
}

KShip *KShipList::shipAt(int x, int y)
{
    int tempx, tempy;
    KShip *shipiterator;
    QListIterator<KShip *> i(m_shiplist);
    while (i.hasNext()) {
        shipiterator = i.next();
        for (tempy = shipiterator->shipystart(); tempy <= shipiterator->shipystop(); tempy++) {
            if (tempy == y) {
                for (tempx = shipiterator->shipxstart(); tempx <= shipiterator->shipxstop(); tempx++) {
                    if (tempx == x)
                        return shipiterator;
                }
            }
        }
    }
    return 0;
}

bool KShipList::canAddShips()
{
    if (m_shipsadded >= 1)
        return true;
    return false;
}

void KShipList::addNewShip(int button, int fieldx, int fieldy)
{
    if (!addNewShip(!(button == Qt::LeftButton), fieldx, fieldy))
        KMessageBox::information(0L, i18n("You cannot place the ship here."));
}

bool KShipList::addNewShip(bool vertical, int fieldx, int fieldy)
{
    QRect ship = vertical ? QRect(fieldx, fieldy, 1, m_shipsadded) : QRect(fieldx, fieldy, m_shipsadded, 1);
    QRect field = QRect(0, 0, m_fieldx, m_fieldy);
    if (!field.contains(ship))
        return false;

    KShip *placedShip;
    QListIterator<KShip *> i(m_shiplist);
    while (i.hasNext()) {
        placedShip = i.next();
        for (int i = fieldx - 1; i < (fieldx + ship.width() + 1); i++) {
            if (placedShip->contains(i, fieldy - 1) || placedShip->contains(i, fieldy + ship.height()))
                return false;
        }

        for (int i = fieldy - 1; i < (fieldy + ship.height() + 1); i++) {
            if (placedShip->contains(fieldx - 1, i) || placedShip->contains(fieldx + ship.width(), i))
                return false;
        }
    }

    m_shipsadded--;

    if (!vertical)
        m_shiplist.append(new KShip(fieldx, fieldx + shipCount(), fieldy, fieldy, shipCount(), true));
    else
        m_shiplist.append(new KShip(fieldx, fieldx, fieldy, fieldy + shipCount(), shipCount(), false));

    for (int i = 0; i < shipCount() + 1; i++) {
        int start = 0;
        if (shipCount() == 3)
            start = KBattleField::SHIP4P1;
        else if (shipCount() == 2)
            start = KBattleField::SHIP3P1;
        else if (shipCount() == 1)
            start = KBattleField::SHIP2P1;
        else if (shipCount() == 0)
            start = KBattleField::SHIP1P1;

        if (!vertical)
            emit sigOwnFieldDataChanged(fieldx + i, fieldy, start + i);
        else
            emit sigOwnFieldDataChanged(fieldx, fieldy + i, start + i);
    }

    if (m_shipsadded == 0)
        emit sigLastShipAdded();
    return true;
}
