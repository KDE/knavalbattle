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

#include <kapplication.h>
#include <kconfig.h>
#include <dnssd/servicebrowser.h>
#include <dnssd/remoteservice.h>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <kdialogbase.h>
#include "connectDlg.h"

class KClientDialog : public KDialogBase
{
	Q_OBJECT
public:
	KClientDialog(QWidget *parent = 0, const char *name = 0);
	~KClientDialog();

	QString port() const;
	QString host() const;
	QString nickname() const;

public slots:
        virtual void slotOk();
        virtual void slotCancel();
	void slotReturnPressed(const QString &hostname);
	void nextBatch();
	void gameSelected(int);	
        void slotCheckEnableOk();

signals:
	void sigConnectServer();
	void sigCancelConnect();

private:
	KConfig *m_config;
	DNSSD::ServiceBrowser *m_browser;
	clientConnectDlg *m_mainWidget;
};

#endif
