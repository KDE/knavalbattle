#ifndef GAMECHOOSER_H
#define GAMECHOOSER_H

#include <QObject>
#include <QSignalMapper>

class WelcomeScreen;
class OptionVisitor;
class GeneralController;
class SeaView;
class ChatWidget;
class Button;

class ChooserOption : public QObject
{
Q_OBJECT
    QString m_nick;
public:
    virtual ~ChooserOption() { }
    
    virtual void apply(OptionVisitor& visitor) = 0;
    virtual void initialize(Button* button) = 0;
    
    QString nick() const;
    virtual bool complete() const = 0;
public slots:
    void setNick(const QString& nick);
signals:
    void done();
};

class HumanChooserOption : public ChooserOption
{
Q_OBJECT
    WelcomeScreen* m_screen;
    Button* m_button;
    bool m_complete;
public:
    HumanChooserOption(WelcomeScreen* screen);
    virtual void apply(OptionVisitor& visitor);
    virtual void initialize(Button* button);
    
    virtual bool complete() const { return m_complete; }
public slots:
    void finalize();
};

class AIChooserOption : public ChooserOption
{
Q_OBJECT
public:
    enum Level
    {
        Undefined,
        Easy,
        Medium,
        Hard
    };
private:
    WelcomeScreen* m_screen;
    Level m_level;
    
    void finalize();
public:
    AIChooserOption(WelcomeScreen* screen);
    
    virtual void apply(OptionVisitor& visitor);
    virtual void initialize(Button* button);
    Level level() const { return m_level; }
    virtual bool complete() const { return m_level != Undefined; }
public slots:
    void setEasy();
    void setMedium();
    void setHard();
};

class NetworkChooserOption : public ChooserOption
{
Q_OBJECT
    WelcomeScreen* m_screen;
    bool m_server;
    QString m_host;
    int m_port;
    
    void finalize();
public:
    NetworkChooserOption(WelcomeScreen* screen);
    virtual void initialize(Button* button);
    virtual void apply(OptionVisitor& visitor);
    virtual bool complete() const { return false; }
    
public slots:
    void setServer();
    void setClient();
    
    void setHost(const QString& host);
    void setPort(const QString& port);
};

class OptionVisitor
{
public:
    virtual ~OptionVisitor() { }
    virtual void visit(const HumanChooserOption&) = 0;
    virtual void visit(const AIChooserOption&) = 0;
    virtual void visit(const NetworkChooserOption&) = 0;
};

class ScreenManager : public QObject
{
Q_OBJECT
    WelcomeScreen* m_screen;
    ChooserOption* m_option;
    
    void setOption(ChooserOption* option, Button* button);
public:
    virtual ~ScreenManager() { }
    
    ScreenManager(QObject* parent, WelcomeScreen* screen);
    
    ChooserOption* option() { return m_option; }
    void removeHumanButton();
private slots:
    void human();
    void ai();
    void network();
    
signals:
    void done();
    void selected();
};

class GameChooser : public QObject
{
Q_OBJECT
    ScreenManager* m_managers[2];
    QSignalMapper m_done_mapper;
    QSignalMapper m_select_mapper;
public:
    GameChooser(QObject* parent, WelcomeScreen* screen0, WelcomeScreen* screen1);
    
    void setupController(GeneralController* controller, SeaView* sea, ChatWidget* chat);
    bool complete() const;
public slots:
    void chosen(int player);
    void selected(int player);
signals:
    void done();
};

#endif // GAMECHOOSER_H

