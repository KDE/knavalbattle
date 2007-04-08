/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "controller.h"

class NetworkController : public Controller
{
Q_OBJECT
    SeaView* m_view;
    Sea::Player m_player;
public:
    NetworkController(QObject* parent, SeaView* view, Sea::Player player);
};

#endif // NETWORKCONTROLLER_H

