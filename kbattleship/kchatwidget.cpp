/***************************************************************************
                              kchatwidget.cpp
                             -----------------
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

#include <kapp.h>

#include "kchatwidget.moc"

KChatWidget::KChatWidget(QWidget *parent, const char *name) : chatDlg(parent, name)
{
    setMinimumSize(600, 180);
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotComputeMessage()));
    connect(commentEdit, SIGNAL(returnPressed()), this, SLOT(slotComputeMessage()));
    currentNickname = "";
    chatView->setFocusProxy(commentEdit);
    commentEdit->installEventFilter(this);
}

KChatWidget::~KChatWidget()
{
}

void KChatWidget::clear()
{
    currentNickname = "";
    acceptMsg(false);
    chatView->clear();
    commentEdit->clear();
}

void KChatWidget::acceptMsg(bool value)
{
    acceptMsgs = value;
}

void KChatWidget::receivedMessage(const QString &nickname, const QString &msg, bool fromenemy)
{
    // Niko Z:
    // IRC roxxx :)
    if(msg.startsWith("/me ")) 
	chatView->append(QString(" * ") + nickname + QString(" ") + msg.mid(4));
    else if(msg.startsWith("/nick "))
	if(fromenemy)
	    emit changeEnemyNickname(msg.mid(6));
	else
	    emit changeOwnNickname(msg.mid(6));
    else
	chatView->append(nickname + QString(": ") + msg);
    chatView->setCursorPosition(chatView->numLines(), 0);
}

void KChatWidget::setNickname(const QString &nickname)
{
    currentNickname = nickname;
}

bool KChatWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == commentEdit && e->type() == QEvent::Wheel)
    {
        kapp->notify(chatView, e);
        return true;
    }
    return chatDlg::eventFilter(obj, e);
}

void KChatWidget::slotComputeMessage()
{
    if(!commentEdit->text().stripWhiteSpace().isEmpty() && acceptMsgs)
    {
	receivedMessage(currentNickname, commentEdit->text(), false);
	emit sendMessage(commentEdit->text());
	commentEdit->setText("");
    }
    else if(commentEdit->text().stripWhiteSpace().isEmpty() && acceptMsgs)
	commentEdit->setText("");
}
