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

KMessage::KMessage( const KMessageType &type ) : QObject()
{
    if( type.hasType() )
        messageType = type.getType();
}

KMessage::~KMessage()
{
}

void KMessage::addField( QString name, QString content )
{
    messageMap.insert( name, content );
}

void KMessage::setDataStream( QString stream )
{
    messageStream = stream;
    parseMessage();
}

void KMessage::parseMessage()
{
    QStringList list;
    QString key, data;
    bool kd=false;
    list=QStringList::split( QString( "|" ), messageStream );
    for( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
    {
        if( !kd )
        {
            if( *it != "end" )
            {
                QChar testchar;
                int testint;
                testint=(*it).toInt();
                testchar=testint;
                if( !testchar.isDigit() )
                {
                    key=*it;
                    kd=true;
                }
                else
                {
                    KMessageType msgtype;
                    msgtype.setType( (*it).toInt() );
                    messageType=msgtype.getType();
                }
            }
            else
                break;
        }
        else
        {
            data=*it;
            messageMap.insert( key, data );
            key="";
            data="";
            kd=false;
        }

    }
}

QString KMessage::returnSendStream()
{
    QString sendStream;
    QMapIterator<QString,QString> it;

    sendStream=messageType + "|";

    for( it = messageMap.begin(); it != messageMap.end(); ++it )
    {
        sendStream=sendStream + it.key() + "|" + it.data() + "|";
    }

    sendStream=sendStream + "end";

    return sendStream;
}

QString KMessage::getField( QString name )
{
    QMapIterator<QString,QString> it;
    for( it = messageMap.begin(); it != messageMap.end(); ++it )
    {
        if( it.key() == name )
        {
            return it.data();
        }
    }
}

int KMessage::getType()
{
    return messageType;
}

