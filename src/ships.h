/*
  Copyright (c) 2013 Jaime Torres <jtamate@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef Ships_H
#define Ships_H

#include <QHash>
#include <QString>
#include "ship.h"

class Ships
{
private:
    unsigned int m_size;       // of the ships
    unsigned int m_number;     // number of ships of this size
    QString m_shipsName;       // the english singular name of the ships of this size
    QString m_shipsPluralName; // the english plural of the ships of this size
public:
    Ships(): m_size(0), m_number(0), m_shipsName(), m_shipsPluralName() { }
    Ships(unsigned int size, unsigned int number, QString& shipsName, QString& shipsPluralName);
    inline unsigned int size() const { return m_size; }
    inline unsigned int number() const { return m_number; }
    inline QString shipsName() const { return m_shipsName; }
    inline QString pluralShipsName() const { return m_shipsPluralName; }
};

// This configuration is interchanged in the network games.
// In the versions previous to KDE SC 4.13, only adjacent and multiple where interchanged.
class BattleShipsConfiguration
{
private:
    unsigned int m_longestShip;
    bool m_allowAdjacentShips;
    unsigned int m_boardWidth;
    unsigned int m_boardHeight;
    bool m_fromXML;
    QHash<unsigned int,Ships> m_ships;
public:

    BattleShipsConfiguration(const bool fromXML=false);
    explicit BattleShipsConfiguration(unsigned int longestShipSize, const bool allowAdjacentShips, const unsigned int boardWidth, const unsigned int boardHeight, const bool fromXML=false);
    BattleShipsConfiguration(const BattleShipsConfiguration& copy);
    // does not add any ship longer than longestShip
    // overwrites any previous configuration for ships of the requested size
    BattleShipsConfiguration& addShips(unsigned int size, unsigned int number, QString shipsName, QString shipsPluralName);
    BattleShipsConfiguration& addShips(Ships &ships);
    unsigned int numberOfShipsOfSize(unsigned int size) const; // 0 if size is invalid
    QString nameOfShipsOfSize(unsigned int size) const; // QString() if size is invalid
    QString pluralNameOfShipsOfSize(unsigned int size) const; // QString() if size is invalid
    bool multipleShips() const; // return true if any ship size has more than one ship

    void setLongestShipSize(unsigned int longestShipSize);
    void setAllowAdjacentShips(const bool allow) { m_allowAdjacentShips = allow; }
    void setBoardWidth(const unsigned int boardWidth) { m_boardWidth = boardWidth; }
    void setBoardHeight(const unsigned int boardHeight) { m_boardWidth = boardHeight; }
    void setFromXML(bool fromXML) { m_fromXML=fromXML; }
    inline unsigned int boardWidth() const { return m_boardWidth; }
    inline unsigned int boardHeight() const { return m_boardHeight; }
    inline bool isAllowedAdjacentShips() const { return m_allowAdjacentShips; }
    inline bool isFromXML() const { return m_fromXML; }
    // ships are of 0 < size <= longestShip()
    inline unsigned int longestShip() const { return m_longestShip; }
    unsigned int totalNumberOfShipsToPlay() const;
    bool isAValidConfiguration() const;

    static BattleShipsConfiguration defaultSingleShipsConfiguration(const bool allowAdjacent, const bool fromXML = false);
    static BattleShipsConfiguration defaultMultipleShipsConfiguration(const bool allowAdjacent, const bool fromXML = false);
    static const BattleShipsConfiguration* constDefaultSingleShipsConfiguration(const bool allowAdjacent, const bool fromXML = false);

};



#endif // Ships_H
