/***************************************************************************
                                  kship.cpp
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

#include "kship.moc"

KShip::KShip(int _shipxstart, int _shipxstop, int _shipystart, int _shipystop, int _shiplength, bool _placedLeft) : QObject()
{
    m_shipxstart = _shipxstart;
    m_shipxstop = _shipxstop;
    m_shipystart = _shipystart;
    m_shipystop = _shipystop;
    m_shiptype = _shiplength;
    m_placedLeft = _placedLeft;
}

KShip::~KShip()
{
}

int KShip::shipxstart()
{
    return m_shipxstart;
}

int KShip::shipxstop()
{
    return m_shipxstop;
}

int KShip::shipystart()
{
    return m_shipystart;
}

int KShip::shipystop()
{
    return m_shipystop;
}

int KShip::shiptype()
{	
    return m_shiptype;
}

bool KShip::placedLeft()
{
    return m_placedLeft;
}

bool KShip::contains(int x, int y)
{
    return (x >= m_shipxstart && x <= m_shipxstop) && (y >= m_shipystart && y <= m_shipystop);
}
