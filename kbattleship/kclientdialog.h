/***************************************************************************
                                   kclientdialog.h
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
 
#ifndef KCLIENTDIALOG_H
#define KCLIENTDIALOG_H

#include <kapp.h>
#include <kconfig.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <stdlib.h>
#include "dialogs/connectDlg.h"

class KClientDialog : public clientConnectDlg
{
    Q_OBJECT
    public:
        KClientDialog(QWidget *parent = 0, const char *name = 0);
        ~KClientDialog();

        QString getPort();
        QString getHost();
        QString getNickname();
    	
    public slots:
        void slotConnectClicked();
        void slotCancelClicked();
	void slotReturnPressed(const QString &hostname);
    
    signals:
        void sigConnectServer();
        void sigCancelConnect();
    
    private:
        KConfig *config;
};

#endif
