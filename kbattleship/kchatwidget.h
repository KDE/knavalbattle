/***************************************************************************
                              kchatwidget.h
                             ---------------
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

#ifndef KCHATWIDGET_H
#define KCHATWIDGET_H

#include <qpushbutton.h>
#include <qmultilineedit.h>
#include <qlineedit.h>
#include "dialogs/chatDlg.h"
#include "kmessage.h"

class KChatWidget : public chatDlg
{
    Q_OBJECT
    public:
        KChatWidget(QWidget *parent = 0, const char *name = 0);
        ~KChatWidget();

	void clear();
        void setNickname(const QString &nickname) { m_currentNickname = nickname; }

    public slots:
        void slotAcceptMsg(bool value);
        void slotComputeMessage();
        void slotReceivedMessage(const QString &nickname, const QString &msg, bool fromenemy = true);
    
    signals:
	void sigSendMessage(const QString &);
	void sigChangeEnemyNickname(const QString &);
	void sigChangeOwnNickname(const QString &);
   
    private:
        virtual bool eventFilter(QObject *, QEvent *);

        QString m_currentNickname;
        bool m_acceptMsgs;
};

#endif
