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

#include "kship.h"

#include "kbattlefield.h"

KShip::KShip(int _shipxstart, int _shipxstop, int _shipystart, int _shipystop, int _shiplength, bool _placedLeft)
{
	m_shipxstart = _shipxstart;
	m_shipxstop = _shipxstop;
	m_shipystart = _shipystart;
	m_shipystop = _shipystop;
	m_shiptype = _shiplength;
	m_placedLeft = _placedLeft;
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

int KShip::shipTypeEnum(int x, int y)
{
	int ret;
	ret = KBattleField::WATER;
	if (contains(x, y))
	{
		switch(m_shiptype)
		{
			case 0:
				ret = KBattleField::SHIP1P1;
				break;
			case 1:
				if (x == m_shipxstart && y == m_shipystart) ret = KBattleField::SHIP2P1;
				else ret = KBattleField::SHIP2P2;
				break;
			case 2:
				if (x == m_shipxstart && y == m_shipystart) ret = KBattleField::SHIP3P1;
				else if (x == m_shipxstop && y == m_shipystop) ret = KBattleField::SHIP3P3;
				else ret = KBattleField::SHIP3P2;
				break;
			case 3:
				if (x == m_shipxstart && y == m_shipystart) ret = KBattleField::SHIP4P1;
				else if (x == m_shipxstop && y == m_shipystop) ret = KBattleField::SHIP4P4;
				else if (placedLeft())
				{
					if (x == m_shipxstart + 1 && y == m_shipystart) ret = KBattleField::SHIP4P2;
					else ret = KBattleField::SHIP4P3;
				}
				else
				{
					if (x == m_shipxstart && y == m_shipystart + 1) ret = KBattleField::SHIP4P2;
					else ret = KBattleField::SHIP4P3;
				}
				break;
		}
	}
	return ret;
}
