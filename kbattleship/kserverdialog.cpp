/***************************************************************************
                              kserverdialog.cpp
                             -------------------
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

#include <stdlib.h>
#include "kserverdialog.moc"

KServerDialog::KServerDialog(QWidget *parent, const char *name) : serverStartDlg(parent, name)
{
    connect(startBtn, SIGNAL(clicked()), this, SLOT(slotStartClicked()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    nicknameEdit->setText(QString::fromLatin1(getenv("LOGNAME")));  
}

KServerDialog::~KServerDialog()
{
}

void KServerDialog::slotStartClicked()
{
    hide();
    emit sigStartServer();
}

void KServerDialog::slotCancelClicked()
{
    hide();
    emit sigCancelServer();
}

QString KServerDialog::port() const
{
    return QString::number(portEdit->value());
}

QString KServerDialog::nickname() const
{
    return nicknameEdit->text();
}
