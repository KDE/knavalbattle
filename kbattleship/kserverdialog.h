/***************************************************************************
                                   kserverdialog.h
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
 
#ifndef KSERVERDIALOG_H
#define KSERVERDIALOG_H

#include <stdlib.h>
#include <kdebug.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include "dialogs/serverDlg.h"

class KServerDialog : public serverStartDlg
{
    Q_OBJECT
    public:
        KServerDialog( QWidget *parent = 0, const char *name = 0 );
        ~KServerDialog();
	
		QString getPort();
	
    public slots:
        void slotStartClicked();
		void slotCancelClicked();

    signals:
		void startServer();	
		void cancelServer();
};

#endif

