/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "connectdialog.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QLayout>
#include <QLabel>

#include <kicon.h>
#include <kpushbutton.h>

ConnectDialog::ConnectDialog(const QString& hostname, int port, QWidget* parent)
: QWidget(parent)
{
    QGridLayout* layout = new QGridLayout;
    
    int rows;
    if (hostname.isEmpty()) {
        m_hostname = 0;
        rows = 1;
    }
    else {
        m_hostname = new QLineEdit(hostname, this);
        connect(m_hostname, SIGNAL(returnPressed()), this, SIGNAL(done()));
        layout->addWidget(m_hostname, 0, 0, 1, 2);
        rows = 2;
    }

    m_port = new QSpinBox(this);
    m_port->setRange(1, 99999);
    m_port->setValue(port);
    layout->addWidget(m_port, rows - 1, 1);
    
    QLabel* lbl = new QLabel("Port:");
    QPalette palette = lbl->palette();
    palette.setColor(QPalette::Foreground, Qt::white);
    lbl->setPalette(palette);
    layout->addWidget(lbl, rows - 1, 0);    
    
    m_ok = new KPushButton(KIcon("ok"), "");
    layout->addWidget(m_ok, 0, 2, rows, 1);

    layout->setMargin(0);
    layout->setSpacing(1);
    setLayout(layout);
    

    connect(m_ok, SIGNAL(pressed()), this, SIGNAL(done()));
}

QString ConnectDialog::hostname() const
{
    if (m_hostname) {
        return m_hostname->text();
    }
    else {
        return "";
    }
}

int ConnectDialog::port() const
{
    return m_port->value();
}


#include "connectdialog.moc"

