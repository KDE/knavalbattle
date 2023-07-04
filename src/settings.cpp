/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "settings.h"

#include <KUser>

QString Settings::findNick()
{
    QString nick = nickname();
    if (nick.isEmpty()) {
        KUser user;
        nick = user.property(KUser::FullName).toString();
        if (nick.isEmpty()) {
            nick = user.loginName();
        }
    }
    
    return nick;
}

#include "moc_settings.cpp"
