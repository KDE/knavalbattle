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

#include <klocale.h>
#include <kuser.h>
#include <QLayout>

#include "kserverdialog.h"
#include "ui_serverDlg.h"

class ServerStartDlg: public QWidget, public Ui::serverStartDlg
{
public:
	ServerStartDlg(QWidget* parent) : QWidget(parent)
	{
		setupUi(this);
	}
};

KServerDialog::KServerDialog(QWidget *parent) : 
	KDialog(parent)
{
	setCaption(i18n("Start Server"));
	setButtons(Ok|Cancel);
	setDefaultButton(Ok);
	setButtonGuiItem(Ok,KGuiItem(i18n("&Start")));
	setModal(true);
	QFrame* page = new QFrame(this);
	setMainWidget(page);
	QGridLayout* pageLayout = new QGridLayout(page);
	pageLayout->setMargin(0);
        pageLayout->setSpacing(0);
	m_mainWidget = new ServerStartDlg(page);
	pageLayout->addWidget(m_mainWidget, 0, 0);
	
	KUser u;
	m_mainWidget->nicknameEdit->setText(u.loginName());

	QString gamename = u.fullName();
	if(gamename.isEmpty()) gamename = u.loginName();
	m_mainWidget->gamenameEdit->setText(gamename);
	connect(this,SIGNAL(okClicked()),this,SLOT(slotOk()));
	connect(this,SIGNAL(cancelClicked()),this,SLOT(slotCancel()));
}

void KServerDialog::slotOk()
{
	hide();
	emit okClicked();
}

void KServerDialog::slotCancel()
{
	hide();
	emit cancelClicked();
}

QString KServerDialog::port() const
{
	return QString::number(m_mainWidget->portEdit->value());
}

QString KServerDialog::nickname() const
{
	return m_mainWidget->nicknameEdit->text();
}

QString KServerDialog::gamename() const
{
	return m_mainWidget->gamenameEdit->text();
}

#include "kserverdialog.moc"
