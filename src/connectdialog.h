/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QWidget>

class QLineEdit;
class QSpinBox;
class KPushButton;

class ConnectDialog : public QWidget
{
Q_OBJECT
    QLineEdit* m_hostname;
    QSpinBox* m_port;
    KPushButton* m_ok;
public:
    ConnectDialog(const QString& hostname, int port, QWidget* parent = 0);
    
    int port() const;
    QString hostname() const;
signals:
    void done();
};

#endif // CONNECTDIALOG_H

