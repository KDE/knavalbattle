/***************************************************************************
                              kclientdialog.cpp
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

#include "kclientdialog.moc"

KClientDialog::KClientDialog( QWidget *parent, const char *name ) : clientConnectDlg( parent, name )
{
    config = kapp->config();
    config->setGroup( "General" );
    connect( connectBtn, SIGNAL( clicked() ), this, SLOT( slotConnectClicked() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( slotCancelClicked() ) );
    nicknameEdit->setText( getenv( "LOGNAME" ) );
    serverEdit->setText( config->readEntry( "lastServer" ) );
}

KClientDialog::~KClientDialog()
{
    config->writeEntry( "lastServer", serverEdit->text() );
    config->sync();
}

void KClientDialog::slotConnectClicked()
{
    this->hide();
    emit connectServer();
}

void KClientDialog::slotCancelClicked()
{
    this->hide();
    emit cancelConnect();
}

QString KClientDialog::getPort()
{
    QString qport;
    qport.setNum( portEdit->value() );
    return qport;
}

QString KClientDialog::getHost()
{
    return serverEdit->text();
}

QString KClientDialog::getNickname()
{
    return nicknameEdit->text();
}
