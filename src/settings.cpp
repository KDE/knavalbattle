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
