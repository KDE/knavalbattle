/***************************************************************************
                                 kmessage.h
                             -------------------
    Developers: (c) 2000 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KMESSAGE_H
#define KMESSAGE_H

#include <kdebug.h>
#include <qmap.h>
#include <qobject.h>
#include <qdom.h>
#include <qstring.h>
#include <qstringlist.h>

class KMessage : public QObject
{
    Q_OBJECT
    public:
        enum{GREET, SHIPLIST, ANSWER_SHOOT, CHAT, REPLAY};
	
        KMessage(int type);
        KMessage();
        ~KMessage();

        int getType();

	void addField(const QString &name, const QString &content);
        QString getField(const QString &name);

        void setDataStream(const QString &stream);
        QString returnSendStream();

        void chatMessage(const QString &nickname, const QString &message);
	void addReplayRequest();
	void addReady();
	void addWinner();
	bool replayRequest();
	bool enemyReady();
	bool enemyWon();

    private:
        QDomDocument *xmlDocument;
        int messageType;
};

#endif
