/*
  Copyright (c) 2013 Jaime Torres <jtamate@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include "ships.h"

Ships::Ships(unsigned int size, unsigned int number, QString& shipsName, QString& shipsPluralName)
:m_size(size)
,m_number(number)
,m_shipsName(shipsName)
,m_shipsPluralName(shipsPluralName)
{
}

BattleShipsConfiguration::BattleShipsConfiguration(const bool fromXML)
: m_longestShip(0)
, m_allowAdjacentShips(true)
, m_boardWidth(0)
, m_boardHeight(0)
, m_fromXML(fromXML)
, m_ships()
{
}

BattleShipsConfiguration::BattleShipsConfiguration(unsigned int longestShipSize, const bool allowAdjacentShips, const unsigned int boardWidth, const unsigned int boardHeight, const bool fromXML)
: m_longestShip(longestShipSize)
, m_allowAdjacentShips(allowAdjacentShips)
, m_boardWidth(boardWidth)
, m_boardHeight(boardHeight)
, m_fromXML(fromXML)
, m_ships()
{
}

BattleShipsConfiguration::BattleShipsConfiguration(const BattleShipsConfiguration& copy)
: m_longestShip(copy.longestShip())
, m_allowAdjacentShips(copy.isAllowedAdjacentShips())
, m_boardWidth(copy.boardWidth())
, m_boardHeight(copy.boardHeight())
, m_fromXML(copy.m_fromXML)
, m_ships(copy.m_ships)
{
}

void BattleShipsConfiguration::setLongestShipSize(unsigned int longestShipSize)
{ 
    m_longestShip = longestShipSize; 
    m_ships.reserve(m_longestShip);
}

BattleShipsConfiguration& BattleShipsConfiguration::addShips(unsigned int size, unsigned int number, QString shipsName, QString shipsPluralName)
{
    if ( size<=m_longestShip )
    {
        Ships toInsert(size, number, shipsName, shipsPluralName);
        m_ships[size]=toInsert;
    }
    return *this;
}

BattleShipsConfiguration& BattleShipsConfiguration::addShips(Ships& ships)
{
    if ( ships.size()<=m_longestShip )
    {
        m_ships[ships.size()]=ships;
    }
    return *this;
}

unsigned int BattleShipsConfiguration::numberOfShipsOfSize(unsigned int size) const
{
    return size <= m_longestShip ? m_ships[size].number() : 0;
}

QString BattleShipsConfiguration::nameOfShipsOfSize(unsigned int size) const
{
    return size <= m_longestShip ? m_ships[size].shipsName() : QString();
}

QString BattleShipsConfiguration::pluralNameOfShipsOfSize(unsigned int size) const
{
    return size <= m_longestShip ? m_ships[size].pluralShipsName() : QString();
}

bool BattleShipsConfiguration::multipleShips() const
{
    bool res = false;
    for (unsigned int i=0; i<m_longestShip; i++)
    {
        res = res || (m_ships[i].number()>1);
    }
    return res;
}

bool BattleShipsConfiguration::isAValidConfiguration() const
{
    if ( m_longestShip==0 || m_boardHeight==0 || m_boardWidth==0 
        || m_longestShip>qMax<unsigned int>(m_boardHeight, m_boardWidth) )
    {
        return false;
    }
    for (unsigned int size=1; size <= m_longestShip; size++)
    {
        if ( m_ships[size].number() ==0 )
        {
            return false;
        }
    }
    return true;
}

unsigned int BattleShipsConfiguration::totalNumberOfShipsToPlay() const
{
    unsigned int sum=0;
    for (unsigned int size=1; size <= m_longestShip; size++)
    {
        sum += m_ships[size].number();
    }
    return sum;
}


BattleShipsConfiguration BattleShipsConfiguration::defaultSingleShipsConfiguration(const bool allowAdjacent, const bool fromXML)
{
    BattleShipsConfiguration res(4, allowAdjacent, 10, 10, fromXML);
    return res.addShips(1, 1, QLatin1String("minesweeper"), QLatin1String("minesweepers"))
              .addShips(2, 1, QLatin1String("frigate"), QLatin1String("frigates"))
              .addShips(3, 1, QLatin1String("cruise"), QLatin1String("cruises"))
              .addShips(4, 1, QLatin1String("carrier"), QLatin1String("carriers"));
}

BattleShipsConfiguration BattleShipsConfiguration::defaultMultipleShipsConfiguration(const bool allowAdjacent, const bool fromXML)
{
    BattleShipsConfiguration res(4, allowAdjacent, 10, 10, fromXML);
    return res.addShips(1, 4, QLatin1String("minesweeper"), QLatin1String("minesweepers"))
              .addShips(2, 3, QLatin1String("frigate"), QLatin1String("frigates"))
              .addShips(3, 2, QLatin1String("cruise"), QLatin1String("cruises"))
              .addShips(4, 1, QLatin1String("carrier"), QLatin1String("carriers"));
}
