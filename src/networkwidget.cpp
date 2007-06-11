#include "networkwidget.h"

#include <QLayout>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>

#include <klineedit.h>
#include <kpushbutton.h>
#include <kseparator.h>

NetworkWidget::NetworkWidget(QWidget* parent)
: QWidget(parent)
{
    QLabel* tmp;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // nickname
    QHBoxLayout* tmpLayout = new QHBoxLayout;
    tmpLayout->addStretch();
    tmp = new QLabel("Nickname ", this);
    tmpLayout->addWidget(tmp);
    m_nickname = new KLineEdit(this);
    tmpLayout->addWidget(m_nickname);
    tmpLayout->addStretch();
    mainLayout->addItem(tmpLayout);
    
    // separator
    KSeparator* sep = new KSeparator(Qt::Horizontal, this);
    mainLayout->addWidget(sep);
    
    // client
//     tmpLayout = new QHBoxLayout;
    QGridLayout *clientLayout = new QGridLayout;
    QGridLayout *clientWLayout = new QGridLayout;
    tmp = new QLabel("Hostname       ", this);
    clientWLayout->addWidget(tmp, 0, 0);
    m_hostname = new KLineEdit(this);
    clientWLayout->addWidget(m_hostname, 0, 1);
//     tmpLayout = new QHBoxLayout;
    tmp = new QLabel("Port       ", this);
    clientWLayout->addWidget(tmp, 1, 0);
//     clientWLayout->addStretch();
    m_port1 = new QSpinBox(this);
    clientWLayout->addWidget(m_port1, 1, 1);
//     mainLayout->addItem(tmpLayout);
//     mainLayout->addItem(clientWLayout);
    clientWLayout->setRowStretch(0,5);
    clientWLayout->setRowStretch(1,5);
//     clientWLayout->setColumnStretch(0,5);
    clientWLayout->setColumnStretch(1,5);

    
//     tmpLayout = new QHBoxLayout;
//     tmpLayout->addStretch();
    m_client_button = new KPushButton("Connect >", this);
    clientLayout->addItem(clientWLayout, 0, 0);
    clientLayout->addWidget(m_client_button, 1, 1);
//     mainLayout->addItem(tmpLayout);
    mainLayout->addItem(clientLayout);
    
    // separator
    sep = new KSeparator(Qt::Horizontal, this);
    mainLayout->addWidget(sep);
    
    // server
    QGridLayout *serverLayout = new QGridLayout;
    QGridLayout *serverWLayout = new QGridLayout;
//     tmpLayout = new QHBoxLayout;
    tmp = new QLabel("Port         ", this);
    serverWLayout->addWidget(tmp, 0, 0);
//     tmpLayout->addStretch();
    m_port2 = new QSpinBox(this);
    serverWLayout->addWidget(m_port2, 0, 1);
    serverWLayout->setRowStretch(0,5);
    serverWLayout->setColumnStretch(1,5);
//     mainLayout->addItem(tmpLayout);
    
//     tmpLayout = new QHBoxLayout;
//     tmpLayout->addStretch();
    m_client_button = new KPushButton("Start server >", this);
    serverLayout->addItem(serverWLayout, 0, 0);
    serverLayout->addWidget(m_client_button, 1, 1);
    mainLayout->addItem(serverLayout);
    
    setLayout(mainLayout);
}


