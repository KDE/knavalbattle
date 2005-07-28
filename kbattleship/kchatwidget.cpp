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

#include <kapplication.h>
#include "kchatwidget.moc"
//Added by qt3to4:
#include <QEvent>

KChatWidget::KChatWidget(QWidget *parent, const char *name) : chatDlg(parent, name)
{
	connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotComputeMessage()));
	connect(commentEdit, SIGNAL(returnPressed()), this, SLOT(slotComputeMessage()));
	chatView->setFocusProxy(commentEdit);
	chatView->setMinimumSize(0, 50);
	commentEdit->installEventFilter(this);

	m_currentNickname = QString::null;
	slotAcceptMsg(false);
}

void KChatWidget::clear()
{
	m_currentNickname = QString::null;
	slotAcceptMsg(false);
	chatView->clear();
	commentEdit->clear();
}

void KChatWidget::slotAcceptMsg(bool value)
{
	m_acceptMsgs = value;
}

void KChatWidget::slotReceivedMessage(const QString &nickname, const QString &msg, bool fromenemy)
{
	// Niko Z:
	// IRC roxxx :)
	if(msg.startsWith("/me "))
		chatView->append(QString(" * ") + nickname + QString(" ") + msg.mid(4));
	else if(msg.startsWith("/nick "))
		if(fromenemy)
			emit sigChangeEnemyNickname(msg.mid(6));
		else
			emit sigChangeOwnNickname(msg.mid(6));
	else
		chatView->append(nickname + QString(": ") + msg);
	chatView->setCursorPosition(chatView->numLines(), 0);
}

bool KChatWidget::eventFilter(QObject *obj, QEvent *e)
{
	if(obj == commentEdit && e->type() == QEvent::Wheel)
	{
		kapp->notify(chatView, e);
		return true;
	}
	return chatDlg::eventFilter(obj, e);
}

void KChatWidget::slotComputeMessage()
{
	if(!commentEdit->text().stripWhiteSpace().isEmpty() && m_acceptMsgs)
	{
		slotReceivedMessage(m_currentNickname, commentEdit->text(), false);
		emit sigSendMessage(commentEdit->text());
		commentEdit->setText("");
	}
	else if(commentEdit->text().stripWhiteSpace().isEmpty() && m_acceptMsgs)
		commentEdit->setText("");
	commentEdit->setFocus();
}
