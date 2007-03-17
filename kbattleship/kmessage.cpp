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

#include <klocale.h>

#ifdef XMLDUMP
#include <kdebug.h>
#endif

#include "kmessage.h"

const char *clientName = I18N_NOOP("KBattleship");
const char *clientVersion = "1.1";
const char *clientDescription = I18N_NOOP("The KDE Battleship clone");
const char *protocolVersion = "0.1.0";

KMessage::KMessage(int type)
{
    m_xmlDocument = QDomDocument("kmessage");
    m_xmlDocument.appendChild(m_xmlDocument.createElement("kmessage"));
    m_messageType = type;
    addField("msgtype", QString::number(type));
}

KMessage::KMessage(KMessage *msg)
{
    m_xmlDocument.setContent(msg->m_xmlDocument.toString(), false);
    m_messageType = msg->type();
}

KMessage::KMessage()
{
    m_xmlDocument = QDomDocument("kmessage");
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
    kDebug() << "*** XML-IN ***" << endl << stream << endl << "***  END  ***" << endl;
#endif
}

QString KMessage::sendStream() const
{
#ifdef XMLDUMP
    kDebug() << "*** XML OUT ***" << endl << m_xmlDocument.toString() << endl << "***  END  ***" << endl;
#endif
    return m_xmlDocument.toString();
}

QString KMessage::field(const QString &name) const
{
    QDomNode xmlNode = m_xmlDocument.documentElement().namedItem(name);
    if (!xmlNode.isNull())
        return (xmlNode.toElement()).text();
    return QString();
}

int KMessage::type()
{
    return field("msgtype").toInt();
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
