/***************************************************************************
                                 kmessage.cpp
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

#include "kmessage.moc"

KMessage::KMessage(int type) : QObject()
{
    xmlDocument = QDomDocument("kmessage");
    xmlDocument.appendChild(xmlDocument.createElement("kmessage"));
    messageType = type;
    addField(QString("msgtype"), QString::number(type));
}

KMessage::KMessage() : QObject()
{
    xmlDocument = QDomDocument("kmessage");
}

KMessage::~KMessage()
{
}

void KMessage::addField(const QString &name, const QString &content)
{
    QDomElement xmlElement = xmlDocument.createElement(name);
    QDomText xmlText = xmlDocument.createTextNode(content);
    xmlElement.appendChild(xmlText);
    xmlDocument.documentElement().appendChild(xmlElement);
}

void KMessage::setDataStream(const QString &stream)
{
    xmlDocument.setContent(stream);
}

QString KMessage::returnSendStream()
{
    return xmlDocument.toString();
}

QString KMessage::getField(const QString &name)
{
    QDomNode xmlNode = xmlDocument.documentElement().firstChild();
    while(!xmlNode.isNull())
    {
	QDomElement xmlElement = xmlNode.toElement();
	if(!xmlElement.isNull())
	    if(xmlElement.tagName() == name)
		return xmlElement.text();
	xmlNode = xmlNode.nextSibling();
    }
    return QString::null;
}

int KMessage::getType()
{
    return getField("msgtype").toInt();
}

void KMessage::chatMessage(const QString &nickname, const QString &message)
{
    addField("nickname", nickname);
    addField("chat", message);
}

void KMessage::addReplayRequest()
{
    addField("enemyW", "replay");
}

void KMessage::addReady()
{
    addField("enemy", "ready");
}

void KMessage::addWinner()
{
    addField("enemyM", "won");
}

bool KMessage::replayRequest()
{
    if(getField("enemyW") == QString("replay"))
	return true;
    return false;
}

bool KMessage::enemyReady()
{
    if(getField("enemy") == QString("ready"))
	return true;
    return false;
}

bool KMessage::enemyWon()
{
    if(getField("enemyM") == QString("won"))
	return true;
    return false;
}
