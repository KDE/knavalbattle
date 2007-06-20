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
    NetworkDialog(bool ask_hostname, QWidget* parent = 0);
    
    QString nickname();
    QString hostname();
    int port();
};

#endif // NETWORKDIALOG_H
