#include "networkdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QSpinBox>

#include <KLineEdit>
#include <KLocalizedString>
#include <KPushButton>
#include <KSeparator>

#include "settings.h"

NetworkDialog::NetworkDialog(bool ask_hostname, QWidget* parent)
: KDialog(parent)
{
    QLabel* tmp;
    QWidget* main = new QWidget(this);
    QHBoxLayout* tmpLayout;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
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
    if (ask_hostname) {
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
    setCaption("Network parameters");
    
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
}

QString NetworkDialog::nickname()
{
    return m_nickname->text();
}

QString NetworkDialog::hostname()
{
    if (m_hostname) {
        return m_hostname->text();
    }
    else {
        return "";
    }
}

int NetworkDialog::port()
{
    return m_port->value();
}

#include "networkdialog.moc"

