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

#include <kdebug.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include "dialogs/connectDlg.h"

class KClientDialog : public clientConnectDlg
{
    Q_OBJECT
    public:
        KClientDialog( QWidget *parent = 0, const char *name = 0 );
        ~KClientDialog();
	
    public slots:
        void slotConnectClicked();
};

#endif

