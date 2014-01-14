/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
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
