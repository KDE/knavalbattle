/***************************************************************************
                           kbattleshipclient.cpp
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


#include "kmessage.h"
#include "kbattleshipclient.moc"

KBattleshipClient::KBattleshipClient(const QString &host, int port) : KStreamSocket(host, QString::number(port))
{
    setBlocking(true);
}

void KBattleshipClient::init()
{
    if (!connect()) {
        emit sigSocketFailure(error());
        return ;
    }

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(slotReadData()));
    QObject::connect(this, SIGNAL(gotError(int)), this, SLOT(sigEndConnect()));
    emit sigConnected();
}

void KBattleshipClient::sendMessage(KMessage *msg)
{
    QByteArray post = msg->sendStream().toUtf8();
    write(post.data(), post.length());
    emit sigMessageSent(msg);
}

void KBattleshipClient::slotReadData()
{
    int len;
    len = bytesAvailable();
    if (!len) {
        emit sigEndConnect();
        return ;
    }

    char *buf = new char[len + 1];
    read(buf, len);
    buf[len] = 0;
    m_readBuffer += QString::fromUtf8(buf);
    delete []buf;
    int pos = m_readBuffer.indexOf("</kmessage>");
    if (pos >= 0) {
        pos += 11; // Length of "</kmessage>"
        KMessage *msg = new KMessage();
        msg->setDataStream(m_readBuffer.left(pos));
        m_readBuffer.remove(0, pos);
        emit sigNewMessage(msg);
    }
}
