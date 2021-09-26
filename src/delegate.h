/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DELEGATE_H
#define DELEGATE_H

class Delegate
{
public:
    virtual ~Delegate() { }
    virtual void action(Sea::Player player, const Coord& c) = 0;
    virtual void changeDirection(Sea::Player player) = 0;
    virtual bool canAddShip(Sea::Player player, const Coord& c) = 0;
    virtual Ship * nextShip() = 0;
};

#endif // DELEGATE_H
