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

//#define XMLDUMP

#include "main.h"
#ifdef XMLDUMP
#include <kdebug.h>
#endif
#include "kmessage.moc"

KMessage::KMessage(int type) : QObject()
{
    m_xmlDocument = QDomDocument("kmessage");
    m_xmlDocument.appendChild(m_xmlDocument.createElement("kmessage"));
    m_messageType = type;
    addField(QString("msgtype"), QString::number(type));
}

KMessage::KMessage() : QObject()
{
    m_xmlDocument = QDomDocument("kmessage");
}

KMessage::~KMessage()
{
}

void KMessage::addField(const QString &name, const QString &content)
{
    QDomElement xmlElement = m_xmlDocument.createElement(name);
    QDomText xmlText = m_xmlDocument.createTextNode(content);
    xmlElement.appendChild(xmlText);
    m_xmlDocument.documentElement().appendChild(xmlElement);
}

void KMessage::setDataStream(const QString &stream)
{
    m_xmlDocument.setContent(stream);
#ifdef XMLDUMP
    kdDebug() << "*** XML-IN ***" << endl << stream << endl << "***  END  ***" << endl;
#endif
}

QString KMessage::returnSendStream()
{
#ifdef XMLDUMP
    kdDebug() << "*** XML OUT ***" << endl << m_xmlDocument.toString() << endl << "***  END  ***" << endl;
#endif
    return m_xmlDocument.toString();
}

QString KMessage::getField(const QString &name)
{
    QDomNode xmlNode = m_xmlDocument.documentElement().firstChild();
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

void KMessage::versionMessage()
{
    addField("protocolVersion", protocolVersion);
    addField("clientName", clientName);
    addField("clientVersion", clientVersion);
    addField("clientDescription", clientDescription);
}
