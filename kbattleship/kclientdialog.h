/***************************************************************************
                                   kclientdialog.h
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
 
#ifndef KCLIENTDIALOG_H
#define KCLIENTDIALOG_H

#include <kapp.h>
#include <kconfig.h>
#include <kdebug.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include "dialogs/connectDlg.h"

class KClientDialog : public clientConnectDlg
{
    Q_OBJECT
    public:
        KClientDialog( QWidget *parent = 0, const char *name = 0 );
        ~KClientDialog();

	QString getPort();
	QString getHost();
		
    public slots:
        void slotConnectClicked();
	void slotCancelClicked();
	
    signals:
	void connectServer();
	void cancelConnect();
	
    private:
	KConfig *config;
};

#endif

