#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>

class KLineEdit;
class QSpinBox;
class KPushButton;

class NetworkWidget : public QWidget
{
    KLineEdit* m_nickname;
    KLineEdit* m_hostname;
    QSpinBox* m_port1;
    QSpinBox* m_port2;
    
    KPushButton* m_client_button;
    KPushButton* m_server_button;
public:
    NetworkWidget(QWidget* parent = 0);
    
    
};

#endif // NETWORKWIDGET_H
