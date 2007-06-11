#include "networkwidget.h"

#include <QLayout>
#include <QLabel>
#include <QSpinBox>

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
    tmpLayout = new QHBoxLayout;
    tmp = new QLabel("Hostname ", this);
    tmpLayout->addWidget(tmp);
    m_hostname = new KLineEdit(this);
    tmpLayout->addWidget(m_hostname);
    mainLayout->addItem(tmpLayout);

    tmpLayout = new QHBoxLayout;
    tmp = new QLabel("Port ", this);
    tmpLayout->addWidget(tmp);
    tmpLayout->addStretch();
    m_port1 = new QSpinBox(this);
    tmpLayout->addWidget(m_port1);
    mainLayout->addItem(tmpLayout);
    
    tmpLayout = new QHBoxLayout;
    tmpLayout->addStretch();
    m_client_button = new KPushButton("Connect >", this);
    tmpLayout->addWidget(m_client_button);
    mainLayout->addItem(tmpLayout);
    
    // separator
    sep = new KSeparator(Qt::Horizontal, this);
    mainLayout->addWidget(sep);
    
    // server
    tmpLayout = new QHBoxLayout;
    tmp = new QLabel("Port ", this);
    tmpLayout->addWidget(tmp);
    tmpLayout->addStretch();
    m_port2 = new QSpinBox(this);
    tmpLayout->addWidget(m_port2);
    mainLayout->addItem(tmpLayout);
    
    tmpLayout = new QHBoxLayout;
    tmpLayout->addStretch();
    m_client_button = new KPushButton("Start server >", this);
    tmpLayout->addWidget(m_client_button);
    mainLayout->addItem(tmpLayout);
    
    setLayout(mainLayout);
}


