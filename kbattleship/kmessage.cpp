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

KMessage::KMessage( int type ) : QObject()
{
    xmlDocument = new QDomDocument( "kmessage" );
    xmlDocument->appendChild(xmlDocument->createElement("kmessage"));
    messageType = type;
    QString qtype;
    qtype.setNum( type );
    addField( QString( "msgtype" ), qtype );
}

KMessage::KMessage() : QObject()
{
}

KMessage::~KMessage()
{
}

void KMessage::addField( QString name, QString content )
{
    kdDebug() << "creating xmlElement" << endl;
    QDomElement xmlElement = xmlDocument->createElement( "message" );
    kdDebug() << "setting Attribute" << endl;
    xmlElement.setAttribute( name, content );
    kdDebug() << "appending to xmlDocElement" << endl;
    xmlDocument->documentElement().appendChild( xmlElement );
    kdDebug() << xmlDocument->toString() << endl;
    parseMessage();
}

void KMessage::setDataStream( QString stream )
{
    xmlDocument->setContent( stream );
    parseMessage();
}

void KMessage::parseMessage()
{
    kdDebug() << "will parse message!" << endl;
    QDomNode xmlNode = xmlDocument->documentElement().firstChild();
    kdDebug() << "1" << endl;
    while( !xmlNode.isNull() )
    {
    kdDebug() << "2"<< endl;
	QDomElement xmlElement = xmlNode.toElement(); 
    kdDebug() << "3" << endl;
	if( !xmlElement.isNull() )
	{
	    kdDebug() << "------- XML Message -------" << endl;
	    kdDebug() << "TagName: " << xmlElement.tagName() << endl;
	    kdDebug() << "Text:    " << xmlElement.text() << endl;
	}
	kdDebug() << "4" << endl;
	xmlNode = xmlNode.nextSibling();
     }
     
/*  QStringList list;
    QString key;
    QString data;
    QString seperator = "|";
    QString endseperator = "end";
    bool kd = false;
    kdDebug() << "Parsing messageStream: " << messageStream << endl;
    list = QStringList::split( seperator, messageStream );
	
    for( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
    {
        if( !kd )
        {
	    key = *it;
    	    kd = true;
        }
        else
        {
	    data = *it;
	    if( key != "msgtype" )
	    {
    		addField( key, data );
	    }
	    else
	    {
                messageType = data.toInt();
	    }
		
    	    key = "";
    	    data = "";
    	    kd = false;
        }

    }
*/
}

QString KMessage::returnSendStream()
{
    QDomNode xmlNode = xmlDocument->documentElement().firstChild();
    
    while( !xmlNode.isNull() )
    {
	QDomElement xmlElement = xmlNode.toElement(); 
	if( !xmlElement.isNull() )
	{
	    kdDebug() << "------- XML Message -------" << endl;
	    kdDebug() << "TagName: " << xmlElement.tagName() << endl;
	    kdDebug() << "Text:    " << xmlElement.text() << endl;
	}
	xmlNode = xmlNode.nextSibling();
     }

    return 0;
}

QString KMessage::getField( QString name )
{
/*
    QMapIterator<QString,QString> it;
    for( it = messageMap.begin(); it != messageMap.end(); ++it )
    {
        if( it.key() == name )
        {
            return it.data();
        }
    }
*/
}

int KMessage::getType()
{
    return messageType;
}

