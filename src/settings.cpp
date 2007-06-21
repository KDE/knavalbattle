/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "settings.h"

#include <KUser>

QString Settings::findNick()
{
    QString nick = nickname();
    if (nick.isEmpty()) {
        KUser user;
        nick = user.fullName();
        if (nick.isEmpty()) {
            nick = user.loginName();
        }
    }
    
    return nick;
}

#include "settings.moc"
