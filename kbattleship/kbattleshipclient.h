/***************************************************************************
                             kbattleshipclient.h
                             -------------------
    Developers: (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KBATTLESHIPCLIENT_H
#define KBATTLESHIPCLIENT_H

#include <kextsock.h>

class QSocketNotifier;
class KMessage;

class KBattleshipClient : public KExtendedSocket
{
    Q_OBJECT
    public:
        KBattleshipClient(const QString &host, int port);
        ~KBattleshipClient();

	void init();

        void sendMessage(KMessage *msg);
        void allowWrite() { writeable = true; }
        void forbidWrite() { writeable = false; }
        bool write() { return writeable; }
    
    private slots:
        void readData();

    signals:
        void connected();
        void senemylist(bool);
        void newMessage(KMessage *);
        void endConnect();
        void socketFailure(int);
     
    private:
        bool writeable;
        QSocketNotifier *readNotifier;
};

#endif
