/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

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
class QTcpSocket;

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
    Button* m_server_button;
    Button* m_client_button;
    bool m_selected;
    
    QTcpSocket* m_socket;

    void setupButtons();
private slots:
    void finalize();
    void processServerConnection();
    void clientError();
public:
    NetworkChooserOption(WelcomeScreen* screen);
    virtual void initialize(Button* button);
    virtual void apply(OptionVisitor& visitor);
    virtual bool complete() const { return m_socket; }
    QTcpSocket* socket() const { return m_socket; }
    
public slots:
    void setServer();
    void startServer();
    void setClient();
    void startClient();
    bool server() const { return m_server; }
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
    
    Button* m_human_button;
    Button* m_ai_button;
    Button* m_network_button;
    
    void setOption(ChooserOption* option, Button* button);
public:
    virtual ~ScreenManager() { }
    
    ScreenManager(QObject* parent, WelcomeScreen* screen);
    
    ChooserOption* option() { return m_option; }
    void removeNonHumanButtons();
public slots:
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

