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

KClientDialog::KClientDialog(QWidget *parent, const char *name) : clientConnectDlg(parent, name)
{
    config = kapp->config();
    nicknameEdit->setText(QString::fromLocal8Bit(getenv("LOGNAME")));

    connect(connectBtn, SIGNAL(clicked()), this, SLOT(slotConnectClicked()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
    connect(serverEdit, SIGNAL(returnPressed(const QString &)), this, SLOT(slotReturnPressed(const QString &)));
    config->setGroup("History");
    serverEdit->completionObject()->setItems(config->readListEntry("CompletionList")); 
    
    serverEdit->setMaxCount(5);
    serverEdit->setHistoryItems(config->readListEntry("HistoryList"));
}

KClientDialog::~KClientDialog()
{
    config->setGroup("History");
    config->writeEntry("CompletionList", serverEdit->completionObject()->items());
    config->writeEntry("HistoryList", serverEdit->historyItems());
    config->sync();
}

void KClientDialog::slotConnectClicked()
{
    if(!serverEdit->currentText().stripWhiteSpace().isEmpty())
    {
	hide();
	serverEdit->addToHistory(serverEdit->currentText());
	emit connectServer();
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
    emit cancelConnect();
}

QString KClientDialog::getPort()
{
    return QString::number(portEdit->value());
}

QString KClientDialog::getHost()
{
    return serverEdit->currentText();
}

QString KClientDialog::getNickname()
{
    return nicknameEdit->text();
}
