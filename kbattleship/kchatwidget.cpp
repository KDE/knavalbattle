/***************************************************************************
                              kchatwidget.cpp
                             -----------------
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

#include "kchatwidget.moc"

KChatWidget::KChatWidget( QWidget *parent, const char *name ) : chatWidget( parent, name )
{
    setMinimumSize( 600, 180 );
    connect( sendBtn, SIGNAL( clicked() ), SLOT( slotComputeMessage() ) );
    connect( commentEdit, SIGNAL( returnPressed() ), SLOT( slotComputeMessage() ) );
    chatView->setReadOnly( true );
}

KChatWidget::~KChatWidget()
{
}

void KChatWidget::acceptMsg( bool value )
{
    acceptMsgs = value;
}

void KChatWidget::receivedMessage( QString nickname, QString msg )
{
    kdDebug() << "Nickname: " << nickname << " Message: " << msg << endl;
    chatView->append( QString( "<" ) + nickname + QString( "> " ) + msg );
    chatView->setCursorPosition( chatView->numLines(), 0 );
}

void KChatWidget::setNickname( QString nickname )
{
    currentNickname = nickname;
}

void KChatWidget::slotComputeMessage()
{
    if( acceptMsgs )
    {
	receivedMessage( currentNickname, commentEdit->text() );
	emit sendMessage( commentEdit->text() );
	commentEdit->setText( "" );
    }
}

