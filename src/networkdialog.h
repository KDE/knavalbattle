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
#include <QUrl>

class KComboBox;
class KPluralHandlingSpinBox;
class KLineEdit;
class QLabel;
class QTcpSocket;
namespace KDNSSD { class PublicService; }

class NetworkDialog : public KDialog
{
Q_OBJECT
    KLineEdit* m_nickname;
    KLineEdit* m_hostname;
    QTcpSocket* m_socket;
    KPluralHandlingSpinBox* m_port;
    QLabel* m_feedback;
    KComboBox* m_games;
    KDNSSD::PublicService* m_publisher;
    bool m_client;
    
    QString hostname() const;
    int port() const;
private slots:
    void savePreferences();
    
    void clientOK();
    void clientError();
    void serverOK();
    void serviceSelected(int idx);

protected:
    void slotButtonClicked(int);
public:
    explicit NetworkDialog(bool client, QWidget* parent = 0, const QUrl* url = 0);
    ~NetworkDialog();
    
    QString nickname() const;
    QTcpSocket* socket() const;
signals:
    void ok();
};

#endif // NETWORKDIALOG_H
