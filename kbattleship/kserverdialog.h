/***************************************************************************
                                   kserverdialog.h
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

#ifndef KSERVERDIALOG_H
#define KSERVERDIALOG_H

#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>

#include <kdialogbase.h>

class ServerStartDlg;

class KServerDialog : public KDialogBase
{
	Q_OBJECT
public:
	KServerDialog(QWidget *parent = 0, const char *name = 0);

	QString port() const;
	QString nickname() const;
	QString gamename() const;

public slots:
	virtual void slotOk();
	virtual void slotCancel();

private:
        ServerStartDlg *m_mainWidget;
};

#endif
