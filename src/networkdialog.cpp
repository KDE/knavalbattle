/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "networkdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QSpinBox>
#include <QTcpSocket>
#include <QTcpServer>

#include <KLineEdit>
#include <KLocalizedString>
#include <KPushButton>
#include <KSeparator>

#include "settings.h"

NetworkDialog::NetworkDialog(bool client, QWidget* parent)
: KDialog(parent)
, m_client(client)
{
    setButtons(Ok | Cancel);

    QLabel* tmp;
    QWidget* main = new QWidget(this);
    QHBoxLayout* tmpLayout;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // feedback
    m_feedback = new QLabel("", this);
    m_feedback->setAlignment(Qt::AlignHCenter);
    {
        QFont font = m_feedback->font();
        font.setStyle(QFont::StyleItalic);
        m_feedback->setFont(font);
    }
    m_feedback->hide();
    mainLayout->addWidget(m_feedback);
    
    // nick
    tmp = new QLabel(i18n("&Nickname:"), main);
    m_nickname = new KLineEdit(main);
    m_nickname->setClearButtonShown(true);
    m_nickname->setText(Settings::findNick());
    tmp->setBuddy(m_nickname);
    tmpLayout = new QHBoxLayout;
    tmpLayout->addWidget(tmp);
    tmpLayout->addStretch();
    tmpLayout->addWidget(m_nickname);
    mainLayout->addItem(tmpLayout);
    
    // hostname
    if (m_client) {
        tmp = new QLabel(i18n("&Hostname:"), main);
        m_hostname = new KLineEdit(main);
        m_hostname->setClearButtonShown(true);
        m_hostname->setText(Settings::hostname());
        tmp->setBuddy(m_hostname);
        tmpLayout = new QHBoxLayout;
        tmpLayout->addWidget(tmp);
        tmpLayout->addStretch();
        tmpLayout->addWidget(m_hostname);
        mainLayout->addItem(tmpLayout);
    }
    else {
        m_hostname = 0;
    }
    
    // port
    tmp = new QLabel(i18n("&Port:"), main);
    m_port = new QSpinBox(main);
    m_port->setRange(1, 99999);
    m_port->setValue(Settings::port());
    tmp->setBuddy(m_port);
    tmpLayout = new QHBoxLayout;
    tmpLayout->addWidget(tmp);
    tmpLayout->addStretch();
    tmpLayout->addWidget(m_port);
    mainLayout->addItem(tmpLayout);
    
    main->setLayout(mainLayout);
    setMainWidget(main);
    setCaption(i18n("Network Parameters"));
    
    connect(this, SIGNAL(accepted()), this, SLOT(savePreferences()));
    
    enableButtonApply(false);
}

void NetworkDialog::savePreferences()
{
    Settings::setNickname(nickname());
    if (m_hostname) {
        Settings::setHostname(hostname());
    }
    Settings::setPort(port());
    Settings::self()->writeConfig();
}

QString NetworkDialog::hostname() const
{
    if (m_hostname) {
        return m_hostname->text();
    }
    else {
        return "";
    }
}

int NetworkDialog::port() const
{
    return m_port->value();
}

QString NetworkDialog::nickname() const
{
    return m_nickname->text();
}

QTcpSocket* NetworkDialog::socket() const
{
    return m_socket;
}

void NetworkDialog::slotButtonClicked(int code)
{
    if (code == Ok) {
        KPushButton* b = button(Ok);
        b->setEnabled(false);
        m_feedback->show();
       
        if (m_client) {
            m_feedback->setText(i18n("Connecting to remote host..."));
            m_socket = new QTcpSocket;
            connect(m_socket, SIGNAL(connected()), this, SLOT(clientOK()));
            connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError()));
            m_socket->connectToHost(m_hostname->text(), m_port->value());
        }
        else {
            m_feedback->setText(i18n("Waiting for an incoming connection..."));        
            QTcpServer* server = new QTcpServer;
            connect(server, SIGNAL(newConnection()), this, SLOT(serverOK()));
            
            server->listen(QHostAddress::Any, static_cast<quint16>(m_port->value()));
        }
    }
    else {
        KDialog::slotButtonClicked(code);
    }
}

void NetworkDialog::clientOK()
{
    accept();
}

void NetworkDialog::clientError()
{
    m_socket->deleteLater();
    m_socket = 0;
    m_feedback->setText(i18n("Could not connect to host"));
    button(Ok)->setEnabled(true);
}

void NetworkDialog::serverOK()
{
    QTcpServer* server = qobject_cast<QTcpServer*>(sender());
    Q_ASSERT(server);
    
    m_socket = server->nextPendingConnection();
    Q_ASSERT(m_socket);
    
    // refuse all other connections
    while (server->hasPendingConnections()) {
        delete server->nextPendingConnection();
    }
    
    // reparent socket, so that we can safely destroy the server
    m_socket->setParent(this);
    delete server;
    
    // we're done
    server->deleteLater();    
    accept();
}

#include "networkdialog.moc"

