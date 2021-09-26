/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2007 Riccardo Iaconelli <ruphy@fsfe.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QDialog>
#include <QUrl>

class KComboBox;
class KPluralHandlingSpinBox;
class KLineEdit;
class QLabel;
class QTcpSocket;
class QPushButton;
namespace KDNSSD { class PublicService; }

class NetworkDialog : public QDialog
{
Q_OBJECT
    QPushButton *m_okButton;
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
private Q_SLOTS:
    void savePreferences();
    
    void clientOK();
    void clientError();
    void serverOK();
    void serviceSelected(int idx);
    void slotOkClicked();
public:
    explicit NetworkDialog(bool client, QWidget* parent = nullptr, const QUrl* url = nullptr);
    ~NetworkDialog() override;
    
    QString nickname() const;
    QTcpSocket* socket() const;
Q_SIGNALS:
    void ok();
};

#endif // NETWORKDIALOG_H
