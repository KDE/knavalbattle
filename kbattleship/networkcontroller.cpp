/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "networkcontroller.h"

NetworkController::NetworkController(QObject* parent, SeaView* view, Sea::Player player)
: Controller(parent)
, m_view(view)
, m_player(player)
{
}

#include "networkcontroller.moc"



