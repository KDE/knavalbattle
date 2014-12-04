/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "networkdialog.h"

#include <QLabel>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <KComboBox>
#include <KPluralHandlingSpinBox>
#include <KLineEdit>
#include <KLocalizedString>
#include <KSeparator>
#include <KDNSSD/DNSSD/ServiceBrowser>
#include <KDNSSD/DNSSD/ServiceModel>
#include <KDNSSD/DNSSD/PublicService>

#include "settings.h"

NetworkDialog::NetworkDialog(bool client, QWidget* parent, const QUrl* url)
: KDialog(parent)
, m_publisher(0), m_client(client)
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
    tmpLayout->addWidget(m_nickname, 1);
    tmpLayout->setSpacing(KDialog::spacingHint());
    mainLayout->addItem(tmpLayout);

    // client part
    if (m_client) {
        tmp = new QLabel(i18n("&Join game:"), main);
        m_games=new KComboBox(main);
        KDNSSD::ServiceBrowser* browser=new KDNSSD::ServiceBrowser("_kbattleship._tcp", true);
        m_games->setModel(new KDNSSD::ServiceModel(browser, this));
        tmp->setBuddy(m_games);
        tmpLayout = new QHBoxLayout;
        tmpLayout->addWidget(tmp);
        tmpLayout->addWidget(m_games, 1);
        tmpLayout->setSpacing(KDialog::spacingHint());
        connect(m_games, static_cast<void (KComboBox::*)(int)>(&KComboBox::currentIndexChanged), this, &NetworkDialog::serviceSelected);
        mainLayout->addItem(tmpLayout);
        
        QWidget* frame=new QWidget(main);
        QVBoxLayout* frameLayout = new QVBoxLayout;
        
        const QString hostName = url? url->host() : Settings::hostname();
        tmp = new QLabel(i18n("&Hostname:"), frame);
        m_hostname = new KLineEdit(main);
        m_hostname->setClearButtonShown(true);
        m_hostname->setText(hostName);
        tmp->setBuddy(m_hostname);
        tmpLayout = new QHBoxLayout;
        tmpLayout->addWidget(tmp);
        tmpLayout->addWidget(m_hostname, 1);
        tmpLayout->setSpacing(KDialog::spacingHint());
        frameLayout->addItem(tmpLayout);

        // port
        const int port = ( url && url->port() != -1 )? url->port(): Settings::port();
        tmp = new QLabel(i18n("&Port:"), main);
        m_port = new KPluralHandlingSpinBox(main);
        m_port->setRange(1, 99999);
        m_port->setValue(port);
        tmp->setBuddy(m_port);
        tmpLayout = new QHBoxLayout;
        tmpLayout->addWidget(tmp);
        tmpLayout->addStretch();
        tmpLayout->addWidget(m_port);
        frameLayout->addItem(tmpLayout);
        frame->setLayout(frameLayout);
        
        mainLayout->addWidget(frame);
        frame->setVisible(false);
        
        QPushButton* sw=new QPushButton(i18n("&Enter server address manually"), main);
        sw->setCheckable(true);
        connect(sw, &QPushButton::toggled, frame, &QWidget::setVisible);
        connect(sw, &QPushButton::toggled, m_games, &KComboBox::setDisabled);
        mainLayout->addWidget(sw);
        if(url) {
            sw->click();
        }
        
    }
    else {
        m_hostname = 0;
        m_games = 0;
    
        // port
        tmp = new QLabel(i18n("&Port:"), main);
        m_port = new KPluralHandlingSpinBox(main);
        m_port->setRange(1, 99999);
        m_port->setValue(Settings::port());
        tmp->setBuddy(m_port);
        tmpLayout = new QHBoxLayout;
        tmpLayout->addWidget(tmp);
        tmpLayout->addStretch();
        tmpLayout->addWidget(m_port);
        mainLayout->addItem(tmpLayout);
    }
    mainLayout->addStretch();
    
    main->setLayout(mainLayout);
    setMainWidget(main);
    setCaption(i18n("Network Parameters"));
    
    connect(this, &NetworkDialog::accepted, this, &NetworkDialog::savePreferences);
    
    enableButtonApply(false);
}

NetworkDialog::~NetworkDialog()
{
    delete m_publisher;
}

void NetworkDialog::savePreferences()
{
    Settings::setNickname(nickname());
    if (m_hostname) {
        Settings::setHostname(hostname());
    }
    Settings::setPort(port());
    Settings::self()->save();
}

void NetworkDialog::serviceSelected(int idx)
{
    if (idx==-1) {
        return;
    }
    KDNSSD::RemoteService::Ptr service=m_games->itemData(idx,KDNSSD::ServiceModel::ServicePtrRole ).value<KDNSSD::RemoteService::Ptr>();
    m_hostname->setText(service->hostName());
    m_port->setValue(service->port());
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
        QPushButton* b = button(Ok);
        b->setEnabled(false);
        m_feedback->show();
       
        if (m_client) {
            m_feedback->setText(i18n("Connecting to remote host..."));
            m_socket = new QTcpSocket;
            connect(m_socket, &QTcpSocket::connected, this, &NetworkDialog::clientOK);
            connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &NetworkDialog::clientError);
            m_socket->connectToHost(m_hostname->text(), m_port->value());
        }
        else {
            m_feedback->setText(i18n("Waiting for an incoming connection..."));        
            QTcpServer* server = new QTcpServer;
            connect(server, &QTcpServer::newConnection, this, &NetworkDialog::serverOK);
            m_publisher=new KDNSSD::PublicService(nickname(), "_kbattleship._tcp", m_port->value());
            m_publisher->publishAsync();
            
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
    accept();
}



