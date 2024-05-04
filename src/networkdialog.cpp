/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2007 Riccardo Iaconelli <ruphy@fsfe.org>

    SPDX-License-Identifier: GPL-2.0-or-later
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
#include <KDNSSD/ServiceBrowser>
#include <KDNSSD/ServiceModel>
#include <KDNSSD/PublicService>
#include <KConfigGroup>
#include <QDialogButtonBox>

#include "settings.h"

NetworkDialog::NetworkDialog(bool client, QWidget* parent, const QUrl* url)
: QDialog(parent)
, m_publisher(nullptr), m_client(client)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    QVBoxLayout *topLayout = new QVBoxLayout;
    setLayout(topLayout);
    m_okButton = buttonBox->button(QDialogButtonBox::Ok);
    m_okButton->setDefault(true);
    m_okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &NetworkDialog::slotOkClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NetworkDialog::reject);

    QLabel* tmp;
    QWidget* main = new QWidget(this);

    QHBoxLayout* tmpLayout;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // feedback
    m_feedback = new QLabel(this);
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
    m_nickname->setClearButtonEnabled(true);
    m_nickname->setText(Settings::findNick());
    tmp->setBuddy(m_nickname);
    tmpLayout = new QHBoxLayout;
    tmpLayout->addWidget(tmp);
    tmpLayout->addWidget(m_nickname, 1);
    mainLayout->addItem(tmpLayout);

    // client part
    if (m_client) {
        tmp = new QLabel(i18n("&Join game:"), main);
        m_games=new KComboBox(main);
        KDNSSD::ServiceBrowser* browser=new KDNSSD::ServiceBrowser(QStringLiteral("_kbattleship._tcp"), true);
        m_games->setModel(new KDNSSD::ServiceModel(browser, this));
        tmp->setBuddy(m_games);
        tmpLayout = new QHBoxLayout;
        tmpLayout->addWidget(tmp);
        tmpLayout->addWidget(m_games, 1);
        connect(m_games, static_cast<void (KComboBox::*)(int)>(&KComboBox::currentIndexChanged), this, &NetworkDialog::serviceSelected);
        mainLayout->addItem(tmpLayout);
        
        QWidget* frame=new QWidget(main);
        QVBoxLayout* frameLayout = new QVBoxLayout;
        
        const QString hostName = url? url->host() : Settings::hostname();
        tmp = new QLabel(i18n("&Hostname:"), frame);
        m_hostname = new KLineEdit(main);
        m_hostname->setClearButtonEnabled(true);
        m_hostname->setText(hostName);
        tmp->setBuddy(m_hostname);
        tmpLayout = new QHBoxLayout;
        tmpLayout->addWidget(tmp);
        tmpLayout->addWidget(m_hostname, 1);
//TODO PORT QT5         tmpLayout->setSpacing(QDialog::spacingHint());
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
        
        auto* sw = new QPushButton(i18nc("@action:button", "&Enter Server Address Manually"), main);
        sw->setCheckable(true);
        connect(sw, &QPushButton::toggled, frame, &QWidget::setVisible);
        connect(sw, &QPushButton::toggled, m_games, &KComboBox::setDisabled);
        mainLayout->addWidget(sw);
        if(url) {
            sw->click();
        }
        
    }
    else {
        m_hostname = nullptr;
        m_games = nullptr;
    
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
    topLayout->addWidget(main);
    topLayout->addWidget(buttonBox);

    setWindowTitle(i18nc("@title:window", "Network Parameters"));
    
    connect(this, &NetworkDialog::accepted, this, &NetworkDialog::savePreferences);
    
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
        return QString();
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

void NetworkDialog::slotOkClicked()
{
        m_okButton->setEnabled(false);
        m_feedback->show();
       
        if (m_client) {
            m_feedback->setText(i18n("Connecting to remote host…"));
            m_socket = new QTcpSocket;
            connect(m_socket, &QTcpSocket::connected, this, &NetworkDialog::clientOK);
            connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkDialog::clientError);
            m_socket->connectToHost(m_hostname->text(), m_port->value());
        }
        else {
            m_feedback->setText(i18n("Waiting for an incoming connection…"));
            QTcpServer* server = new QTcpServer;
            connect(server, &QTcpServer::newConnection, this, &NetworkDialog::serverOK);
            m_publisher=new KDNSSD::PublicService(nickname(), QStringLiteral("_kbattleship._tcp"), m_port->value());
            m_publisher->publishAsync();
            
            server->listen(QHostAddress::Any, static_cast<quint16>(m_port->value()));
        }
}

void NetworkDialog::clientOK()
{
    accept();
}

void NetworkDialog::clientError()
{
    m_socket->deleteLater();
    m_socket = nullptr;
    m_feedback->setText(i18n("Could not connect to host"));
    m_okButton->setEnabled(true);
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

#include "moc_networkdialog.cpp"
