/***************************************************************************
                                 kmessage.h
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

#ifndef KMESSAGE_H
#define KMESSAGE_H

#include <qdom.h>
#include <qstring.h>

class KMessage
{
public:
	enum{GETVERSION, DISCARD, GREET, SHIPSREADY, SHOOT, ANSWER_SHOOT, WON, REPLAY, CHAT};

	KMessage(int type);
	KMessage(KMessage *msg);
	KMessage();

	int type();

	void addField(const QString &name, const QString &content);
	QString field(const QString &name) const;

	void setDataStream(const QString &stream);
	QString sendStream() const;

	void chatMessage(const QString &nickname, const QString &message);
	void versionMessage();

private:
	QDomDocument m_xmlDocument;
	int m_messageType;
};

#endif
