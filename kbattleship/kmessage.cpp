/***************************************************************************
                                 kmessage.cpp
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

#include "kmessage.moc"

KMessage::KMessage( int type )
{
    xmlDocument = new QDomDocument( "kmessage" );
    xmlDocument->appendChild( xmlDocument->createElement( "kmessage" ) );
    messageType = type;
    QString qtype;
    qtype.setNum( type );
    addField( QString( "msgtype" ), qtype );
}

KMessage::KMessage()
{
    xmlDocument = new QDomDocument( "kmessage" );
}

KMessage::~KMessage()
{
}

void KMessage::addField( QString name, QString content )
{
    QDomElement xmlElement = xmlDocument->createElement( name );
    QDomText xmlText = xmlDocument->createTextNode( content );
    xmlElement.appendChild( xmlText );
    xmlDocument->documentElement().appendChild( xmlElement );
}

void KMessage::chatMessage( QString nickname, QString message )
{
    addField( "nickname", nickname );
    addField( "chat", message );
}

void KMessage::setDataStream( QString stream )
{
    xmlDocument->setContent( stream );
}

QString KMessage::returnSendStream()
{
    QString sendStream;
    QDomNode xmlNode = xmlDocument->documentElement().firstChild();
    while( !xmlNode.isNull() )
    {
	QDomElement xmlElement = xmlNode.toElement();
	if( !xmlElement.isNull() )
	    sendStream = xmlDocument->toString();
	xmlNode = xmlNode.nextSibling();
    }
    return sendStream.simplifyWhiteSpace();
}

QString KMessage::getField( QString name )
{
    QDomNode xmlNode = xmlDocument->documentElement().firstChild();
    while( !xmlNode.isNull() )
    {
	QDomElement xmlElement = xmlNode.toElement();
	if( !xmlElement.isNull() )
	    if( xmlElement.tagName() == name )
		return xmlElement.text();
	xmlNode = xmlNode.nextSibling();
    }
    return QString::null;
}

int KMessage::getType()
{
    return getField( "msgtype" ).toInt();
}

