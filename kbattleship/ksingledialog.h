/***************************************************************************
  ksingledialog.h
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

#ifndef KSINGLEDIALOG_H
#define KSINGLEDIALOG_H

#include <qstring.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include "dialogs/singleDlg.h"

class KSingleDialog : public singleStartDlg
{
	Q_OBJECT
public:
	KSingleDialog(QWidget *parent = 0, const char *name = 0);
	~KSingleDialog();

	QString nickname() const;

public slots:
	void slotStartClicked();
	void slotCancelClicked();

signals:
	void sigStartGame();	
	void sigCancelGame();
};

#endif
