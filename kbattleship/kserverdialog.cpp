/***************************************************************************
                              kserverdialog.cpp
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

#include "kserverdialog.moc"

KServerDialog::KServerDialog( QWidget *parent, const char *name ) : serverStartDlg( parent, name )
{
    connect( startBtn, SIGNAL( clicked() ), this, SLOT( slotStartClicked() ) );
}

KServerDialog::~KServerDialog()
{
}

void KServerDialog::slotStartClicked()
{
    kdDebug() << "START!" << endl;
    this->hide();
    emit startServer();
}
