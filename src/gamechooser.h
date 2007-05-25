#ifndef GAMECHOOSER_H
#define GAMECHOOSER_H

#include <QObject>
#include <QSignalMapper>

class WelcomeScreen;
class OptionVisitor;
class GeneralController;
class SeaView;
class ChatWidget;

class ChooserOption : public QObject
{
Q_OBJECT
    QString m_nick;
public:
    virtual ~ChooserOption() { }
    
    virtual void apply(OptionVisitor& visitor) = 0;
    virtual void initialize() = 0;
    
    QString nick() const;
public slots:
    void setNick(const QString& nick);
signals:
    void done();
};

class HumanChooserOption : public ChooserOption
{
Q_OBJECT
    WelcomeScreen* m_screen;
public:
    HumanChooserOption(WelcomeScreen* screen);
    virtual void apply(OptionVisitor& visitor);
    virtual void initialize();
};

class AIChooserOption : public ChooserOption
{
Q_OBJECT
public:
    enum Level
    {
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
    virtual void initialize();
    Level level() const { return m_level; }
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
    virtual void initialize();
    virtual void apply(OptionVisitor& visitor);
    
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
    
    void setOption(ChooserOption* option);
public:
    virtual ~ScreenManager() { }
    
    ScreenManager(QObject* parent, WelcomeScreen* screen);
    
    ChooserOption* option() { return m_option; }
private slots:
    void human();
    void ai();
    void network();
    
signals:
    void done();
};

class GameChooser : public QObject
{
Q_OBJECT
    ScreenManager* m_managers[2];
    QSignalMapper m_mapper;
public:
    GameChooser(QObject* parent, WelcomeScreen* screen0, WelcomeScreen* screen1);
    
    void setupController(GeneralController* controller, SeaView* sea, ChatWidget* chat);
    bool complete() const;
public slots:
    void choose(int player);
signals:
    void done();
};

#endif // GAMECHOOSER_H

