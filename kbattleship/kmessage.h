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

class KMessage : QObject
{
    Q_OBJECT
    public:
		enum{ ENEMY_SHOOT, ANSWER_SHOOT };
        KMessage( int type );
		KMessage();
        ~KMessage();

        void addField( QString name, QString value );
		void chatMessage( QString message );
        QString getField( QString name );
        void setDataStream( QString stream );
        int getType();
        QString returnSendStream();

	protected:
		QDomDocument *xmlDocument;
        int messageType;

};

#endif
