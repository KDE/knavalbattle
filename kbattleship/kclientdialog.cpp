/***************************************************************************
                              kclientdialog.cpp
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

#include <qstring.h>
#include <kcombobox.h>
#include "kclientdialog.moc"

static const char* BATTLESHIP_SERVICE = "_kbattleship._tcp";

KClientDialog::KClientDialog(QWidget *parent, const char *name) : clientConnectDlg(parent, name)
{
	m_config = kapp->config();
	nicknameEdit->setText(QString::fromLocal8Bit(getenv("LOGNAME")));

	connect(connectBtn, SIGNAL(clicked()), this, SLOT(slotConnectClicked()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
	connect(serverEdit, SIGNAL(returnPressed(const QString &)), this, SLOT(slotReturnPressed(const QString &)));
	m_config->setGroup("History");
	m_browser = new DNSSD::ServiceBrowser(BATTLESHIP_SERVICE);
	connect(m_browser,SIGNAL(finished()),SLOT(nextBatch()));
	m_browser->startBrowse();
	connect(lanBox,SIGNAL(activated(int)),SLOT(gameSelected(int)));
	serverEdit->completionObject()->setItems(m_config->readListEntry("CompletionList")); 

	serverEdit->setMaxCount(5);
	serverEdit->setHistoryItems(m_config->readListEntry("HistoryList"));

	serverEdit->setCurrentItem(m_config->readNumEntry("Index", -1));
}

KClientDialog::~KClientDialog()
{
	m_config->setGroup("History");
	m_config->writeEntry("CompletionList", serverEdit->completionObject()->items());
	m_config->writeEntry("HistoryList", serverEdit->historyItems());
	m_config->writeEntry("Index", serverEdit->currentItem());
	m_config->sync();
}

void KClientDialog::slotConnectClicked()
{
	if(!serverEdit->currentText().stripWhiteSpace().isEmpty())
	{
		hide();
		serverEdit->addToHistory(serverEdit->currentText());
		emit sigConnectServer();
	}
	else
		serverEdit->clearEdit();
}

void KClientDialog::slotReturnPressed(const QString &hostname)
{
	if(!hostname.stripWhiteSpace().isEmpty())
		serverEdit->addToHistory(hostname);
	else
		serverEdit->clearEdit();
}

void KClientDialog::slotCancelClicked()
{
	hide();
	emit sigCancelConnect();
}

QString KClientDialog::port() const
{
	return QString::number(portEdit->value());
}

QString KClientDialog::host() const
{
	return serverEdit->currentText();
}

QString KClientDialog::nickname() const
{
	return nicknameEdit->text();
}

void KClientDialog::nextBatch() 
{
	bool autoselect=false;
	if (!lanBox->count()) autoselect=true;
	lanBox->clear();
	QStringList names;
	QValueList<DNSSD::RemoteService::Ptr>::ConstIterator itEnd = m_browser->services().end();
	for (QValueList<DNSSD::RemoteService::Ptr>::ConstIterator it = m_browser->services().begin();
		it!=itEnd; ++it) names << (*it)->serviceName();
	lanBox->insertStringList(names);
	if (autoselect && lanBox->count()) gameSelected(0);
}

void KClientDialog::gameSelected(int i) 
{
	DNSSD::RemoteService::Ptr srv = m_browser->services()[i];
	if (!srv->isResolved() && !srv->resolve()) return;
	serverEdit->setCurrentItem(srv->hostName(),true);
	portEdit->setValue(srv->port());
}
