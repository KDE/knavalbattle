#ifndef SIMPLEMENU_H
#define SIMPLEMENU_H

#include <QObject>

class WelcomeScreen;
class GeneralController;
class SeaView;
class ChatWidget;
class Button;
class QTcpSocket;

class SimpleMenu : public QObject
{
Q_OBJECT
    WelcomeScreen* m_screen;
    
    Button* m_local_game_btn;
    Button* m_server_btn;
    Button* m_client_btn;
    
    QTcpSocket* m_socket;
    
    enum State
    {
        READY,
        DONE_LOCAL_GAME,
        DONE_SERVER,
        DONE_CLIENT
    } m_state;
    void finalize(State);
public:
    SimpleMenu(QWidget* parent, WelcomeScreen* screen);
    
    void setupController(GeneralController* controller,
        SeaView* sea, ChatWidget* chat);
private slots:
    void localGame();
    
    void createServer();
    void processServerConnection();

    void createClient();
    void clientError();
    void clientOK();
signals:
    void done();
};

#endif // SIMPLEMENU_H
