/***************************************************************************
                              kchatwidget.h
                             ---------------
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
	
        void setNickname(QString nickname);

    public slots:
        void acceptMsg(bool value);
        void slotComputeMessage();
        void receivedMessage(QString nickname, QString msg);
    
    signals:
        void sendMessage(QString);
    
    private:
        QString currentNickname;
        bool acceptMsgs;
};

#endif
