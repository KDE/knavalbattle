/***************************************************************************
                              kclientdialog.cpp
                             -------------------
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

#include "kclientdialog.moc"

KClientDialog::KClientDialog( QWidget *parent, const char *name ) : clientConnectDlg( parent, name )
{
    connect( connectBtn, SIGNAL( clicked() ), this, SLOT( slotConnectClicked() ) );
}

KClientDialog::~KClientDialog()
{
}

void KClientDialog::slotConnectClicked()
{
    kdDebug() << "CONNECT!" << endl;
}
