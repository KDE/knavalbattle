/***************************************************************************
                              ksingledialog.cpp
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
#include "ksingledialog.moc"

KSingleDialog::KSingleDialog(QWidget *parent, const char *name) : singleStartDlg(parent, name)
{
	connect(startBtn, SIGNAL(clicked()), this, SLOT(slotStartClicked()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
	nicknameEdit->setText(QString::fromLocal8Bit(getenv("LOGNAME")));  
}

KSingleDialog::~KSingleDialog()
{
}

void KSingleDialog::slotStartClicked()
{
	hide();
	emit sigStartGame();
}

void KSingleDialog::slotCancelClicked()
{
	hide();
	emit sigCancelGame();
}

QString KSingleDialog::nickname() const
{
	return nicknameEdit->text();
}
