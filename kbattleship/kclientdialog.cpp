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
#include <qglobal.h>
//Added by qt3to4:
#include <QGridLayout>
#include <kdebug.h>
#include <klocale.h>
#include <kcombobox.h>
#include <kuser.h>
#include <qlayout.h>
#include <kglobal.h>
#include "kbattleshipserver.h" // for BATTLESHIP_SERVICE
#include "kclientdialog.moc"

KClientDialog::KClientDialog(QWidget *parent, const char *name) 
 : KDialogBase(Plain, i18n("Connect to Server"), Ok|Cancel, Ok, parent, name, true, false, KGuiItem(i18n("&Connect")))
{
	QFrame* page = plainPage();
	QGridLayout* pageLayout = new QGridLayout(page, 1, 1, 0, 0);
	m_mainWidget = new clientConnectDlg(page);
	pageLayout->addWidget(m_mainWidget, 0, 0);

	enableButtonOK(false);
	m_config = KGlobal::config();
	KUser u;
	m_mainWidget->nicknameEdit->setText(u.loginName());

	connect(m_mainWidget->serverEdit, SIGNAL(returnPressed(const QString &)), this, SLOT(slotReturnPressed(const QString &)));
	connect(m_mainWidget->serverEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotCheckEnableOk()));
	
	m_config->setGroup("History");
	m_browser = new DNSSD::ServiceBrowser(QString::fromLatin1(BATTLESHIP_SERVICE));
	connect(m_browser,SIGNAL(finished()),SLOT(nextBatch()));
	m_browser->startBrowse();
	connect(m_mainWidget->lanBox,SIGNAL(activated(int)),SLOT(gameSelected(int)));
	m_mainWidget->serverEdit->completionObject()->setItems(m_config->readEntry("CompletionList",QStringList())); 

	m_mainWidget->serverEdit->setMaxCount(5);
	m_mainWidget->serverEdit->setHistoryItems(m_config->readEntry("HistoryList",QStringList()));

	m_mainWidget->serverEdit->setCurrentItem(m_config->readEntry("Index", -1));
}

KClientDialog::~KClientDialog()
{
	m_config->setGroup("History");
	m_config->writeEntry("CompletionList", m_mainWidget->serverEdit->completionObject()->items());
	m_config->writeEntry("HistoryList", m_mainWidget->serverEdit->historyItems());
	m_config->writeEntry("Index", m_mainWidget->serverEdit->currentItem());
	m_config->sync();
}

void KClientDialog::slotCheckEnableOk()
{
	enableButtonOK(!m_mainWidget->serverEdit->currentText().trimmed().isEmpty());
}

void KClientDialog::slotOk()
{
	QString server = m_mainWidget->serverEdit->currentText().trimmed();
	if(!server.isEmpty())
	{
		hide();
		m_mainWidget->serverEdit->addToHistory(server);
		emit sigConnectServer();
	}
	else
		m_mainWidget->serverEdit->clearEdit();
}

void KClientDialog::slotReturnPressed(const QString &hostname)
{
	if(!hostname.trimmed().isEmpty())
		m_mainWidget->serverEdit->addToHistory(hostname);
	else
		m_mainWidget->serverEdit->clearEdit();
}

void KClientDialog::slotCancel()
{
	hide();
	emit sigCancelConnect();
}

QString KClientDialog::port() const
{
	return QString::number(m_mainWidget->portEdit->value());
}

QString KClientDialog::host() const
{
	return m_mainWidget->serverEdit->currentText();
}

QString KClientDialog::nickname() const
{
	return m_mainWidget->nicknameEdit->text();
}

void KClientDialog::nextBatch() 
{
	bool autoselect=false;
	if (!m_mainWidget->lanBox->count()) autoselect=true;
	m_mainWidget->lanBox->clear();
	QStringList names;
	QList<DNSSD::RemoteService::Ptr>::ConstIterator itEnd = m_browser->services().end();
	for (QList<DNSSD::RemoteService::Ptr>::ConstIterator it = m_browser->services().begin();
		it!=itEnd; ++it) names << (*it)->serviceName();
	m_mainWidget->lanBox->insertStringList(names);
	if (autoselect && m_mainWidget->lanBox->count()) gameSelected(0);
}

void KClientDialog::gameSelected(int i) 
{
	Q_ASSERT(i < m_browser->services().count()); if( i >= m_browser->services().count()) { slotCheckEnableOk(); return; }
	
	DNSSD::RemoteService::Ptr srv = m_browser->services()[i];
	
	Q_ASSERT(srv); if(!srv) { slotCheckEnableOk(); return; }
	
	if (!srv->isResolved() && !srv->resolve()) return;
	m_mainWidget->serverEdit->setCurrentItem(srv->hostName(),true);
	m_mainWidget->portEdit->setValue(srv->port());
	slotCheckEnableOk();
}


