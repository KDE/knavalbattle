/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <KDialog>

class KLineEdit;
class QSpinBox;
class KPushButton;

class NetworkDialog : public KDialog
{
Q_OBJECT
    KLineEdit* m_nickname;
    KLineEdit* m_hostname;
    QSpinBox* m_port;
private slots:
    void savePreferences();
public:
    explicit NetworkDialog(bool ask_hostname, QWidget* parent = 0);
    
    QString nickname();
    QString hostname();
    int port();
};

#endif // NETWORKDIALOG_H
