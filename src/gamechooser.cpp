/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "gamechooser.h"

#include <kdebug.h>
#include <klocale.h>
#include <kicon.h>
#include <kuser.h>
#include <QLineEdit>
#include <QTcpServer>
#include <QTcpSocket>

#include "button.h"
#include "chatwidget.h"
#include "connectdialog.h"
#include "generalcontroller.h"
#include "playerentity.h"
#include "sea.h"

class LineEditorFactory : public EditorFactory
{
    QString m_text;
public:
    LineEditorFactory(const QString& text)
    : m_text(text)
    {
    }
    
    virtual QWidget* createEditor(QWidget* parent)
    {
        return new QLineEdit(m_text, parent);
    }
};

class ConnectDialogFactory : public EditorFactory
{
    QString m_hostname;
    int m_port;
public:
    ConnectDialogFactory(const QString& hostname, int port)
    : m_hostname(hostname)
    , m_port(port)
    {
    }
    
    virtual QWidget* createEditor(QWidget* parent)
    {
        return new ConnectDialog(m_hostname, m_port, parent);
    }
};


void ChooserOption::setNick(const QString& nick)
{
    m_nick = nick;
}

QString ChooserOption::nick() const
{
    return m_nick;
}

HumanChooserOption::HumanChooserOption(WelcomeScreen* screen)
: m_screen(screen)
, m_button(0)
, m_complete(false)
{
}

void HumanChooserOption::initialize(Button* button)
{
    m_button = button;
    
    // read username
    KConfigGroup config(KGlobal::config(), "");
    QString username;
    if (!config.hasKey("nickname")) {
        KUser user;
        username = user.loginName();
    }
    else {
        username = config.readEntry("nickname");
    }
    
    LineEditorFactory factory(username);
    m_button->setEditor(factory);
    QLineEdit* editor = qobject_cast<QLineEdit*>(button->editor());
    Q_ASSERT(editor);
    connect(editor, SIGNAL(returnPressed()), this, SLOT(finalize()));
}

void HumanChooserOption::finalize()
{
    Q_ASSERT(m_button);
    
    QLineEdit* editor = qobject_cast<QLineEdit*>(m_button->editor());
    Q_ASSERT(editor);
    setNick(editor->text());
    m_button->removeEditor();
    
    // save username
    KConfigGroup config(KGlobal::config(), "");
    config.writeEntry("nickname", nick());
    
    m_screen->fadeOut();
    m_complete = true;
    emit done();
}

void HumanChooserOption::apply(OptionVisitor& visitor)
{
    visitor.visit(*this);
}

AIChooserOption::AIChooserOption(WelcomeScreen* screen)
: m_screen(screen)
, m_level(Undefined)
{
}

void AIChooserOption::initialize(Button*)
{
    m_screen->clearButtons();
    Button* button;
     
    button = m_screen->addButton(0, 0, QIcon(), i18n("Easy"));
    connect(button, SIGNAL(clicked()), this, SLOT(setEasy()));
    
    button = m_screen->addButton(0, 1, QIcon(), i18n("Medium"));
    connect(button, SIGNAL(clicked()), this, SLOT(setMedium()));
    
    button = m_screen->addButton(0, 2, QIcon(), i18n("Hard"));
    connect(button, SIGNAL(clicked()), this, SLOT(setHard()));
}

void AIChooserOption::apply(OptionVisitor& visitor)
{
    visitor.visit(*this);
}

void AIChooserOption::setEasy()
{
    m_level = Easy;
    finalize();
}

void AIChooserOption::setMedium()
{
    m_level = Medium;
    finalize();
}

void AIChooserOption::setHard()
{
    m_level = Hard;
    finalize();
}

void AIChooserOption::finalize()
{
    m_screen->fadeOut();
    emit done();
}

NetworkChooserOption::NetworkChooserOption(WelcomeScreen* screen)
: m_screen(screen)
, m_server(false)
, m_socket(0)
{
    m_selected = false;
    m_server_button = 0;
    m_client_button = 0;
}

void NetworkChooserOption::initialize(Button*)
{
    m_screen->clearButtons();
    setupButtons();
}

void NetworkChooserOption::setupButtons()
{
    m_server_button = m_screen->addButton(0, 0, QIcon(), i18n("Wait for a connection"));
    connect(m_server_button, SIGNAL(clicked()), this, SLOT(setServer()));
    
    m_client_button = m_screen->addButton(0, 1, QIcon(), i18n("Connect to a server"));
    connect(m_client_button, SIGNAL(clicked()), this, SLOT(setClient()));
}

void NetworkChooserOption::apply(OptionVisitor& visitor)
{
    visitor.visit(*this);
}

void NetworkChooserOption::setServer()
{
    if (!m_selected) {
        m_server = true;
        m_selected = true;
        KConfigGroup config(KGlobal::config(), "");
        ConnectDialogFactory factory("", config.readEntry("port", 54321));
        m_server_button->setEditor(factory);
        ConnectDialog* editor = qobject_cast<ConnectDialog*>(m_server_button->editor());
        Q_ASSERT(editor);
        connect(editor, SIGNAL(done()), this, SLOT(startServer()));
    }
}

void NetworkChooserOption::startServer()
{
    Q_ASSERT(m_server);
    ConnectDialog* editor = qobject_cast<ConnectDialog*>(m_server_button->editor());
    Q_ASSERT(editor);

    QTcpServer* server = new QTcpServer;
    connect(server, SIGNAL(newConnection()), this, SLOT(processServerConnection()));
    server->listen(QHostAddress::Any, static_cast<quint16>(editor->port()));
    
    m_screen->clearButtons();
    
    // TODO: display some animation to distract the user :)
}

void NetworkChooserOption::setClient()
{
    if (!m_selected) {
        m_server = false;
        m_selected = true;
        KConfigGroup config(KGlobal::config(), "");
        ConnectDialogFactory factory(config.readEntry("hostname", "localhost"),
                                    config.readEntry("port", 54321));
        m_client_button->setEditor(factory);
        ConnectDialog* editor = qobject_cast<ConnectDialog*>(m_client_button->editor());
        Q_ASSERT(editor);
        connect(editor, SIGNAL(done()), this, SLOT(startClient()));
    }
}

void NetworkChooserOption::startClient()
{
    Q_ASSERT(!m_server);
    ConnectDialog* editor = qobject_cast<ConnectDialog*>(m_client_button->editor());
    Q_ASSERT(editor);
    
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()), this, SLOT(finalize()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError()));
    m_socket->connectToHost(editor->hostname(), static_cast<quint16>(editor->port()));
    
    m_screen->clearButtons();   
}

void NetworkChooserOption::processServerConnection()
{
    kDebug() << "incoming connection" << endl;
    
    QTcpServer* server = qobject_cast<QTcpServer*>(sender());
    Q_ASSERT(server);
    
    m_socket = server->nextPendingConnection();
    Q_ASSERT(m_socket);
    
    // refuse all other connections
    while (server->hasPendingConnections()) {
        delete server->nextPendingConnection();
    }
    
    // reparent socket, so that we can safely destroy the server
    m_socket->setParent(this);
    delete server;
    
    // we're done
    kDebug() << "finalizing" << endl;
    finalize();
}

void NetworkChooserOption::clientError()
{
    // restore buttons
    setupButtons();
    
    // reset internal state
    delete m_socket;
    m_socket = 0;
}

void NetworkChooserOption::finalize()
{
    m_screen->fadeOut();
    emit done();
}

ScreenManager::ScreenManager(QObject* parent, WelcomeScreen* screen)
: QObject(parent)
, m_screen(screen)
, m_option(0)
{
    // create buttons
    Button* button;
    button = screen->addButton(0, 0, KIcon("user-female"), i18n("You"));
    Q_ASSERT(button);
    connect(button, SIGNAL(clicked()), this, SLOT(human()));
    
    button = screen->addButton(0, 1, KIcon("roll"), i18n("Computer"));
    Q_ASSERT(button);
    connect(button, SIGNAL(clicked()), this, SLOT(ai()));
    
    button = screen->addButton(0, 2, KIcon("network"), i18n("Over the network"));
    Q_ASSERT(button);
    connect(button, SIGNAL(clicked()), this, SLOT(network()));
}

void ScreenManager::human()
{
    setOption(new HumanChooserOption(m_screen), qobject_cast<Button*>(sender()));
}

void ScreenManager::ai()
{
    setOption(new AIChooserOption(m_screen), qobject_cast<Button*>(sender()));
}

void ScreenManager::network()
{
    setOption(new NetworkChooserOption(m_screen), qobject_cast<Button*>(sender()));
}

void ScreenManager::removeHumanButton()
{
    m_screen->removeButton(0, 0);
    m_screen->moveButton(0, 1, 0, 0);
    m_screen->moveButton(0, 2, 0, 1);
}


void ScreenManager::setOption(ChooserOption* option, Button* button)
{
    if (!m_option) {
        m_option = option;
        connect(m_option, SIGNAL(done()), this, SIGNAL(done()));
        kDebug() << "initializing option" << endl;
        option->initialize(button);
        emit selected();
    }
}

GameChooser::GameChooser(QObject* parent, WelcomeScreen* screen0, WelcomeScreen* screen1)
: QObject(parent)
{
    m_managers[0] = new ScreenManager(this, screen0);
    m_managers[1] = new ScreenManager(this, screen1);
    
    for (int i = 0; i < 2; i++) {
        connect(m_managers[i], SIGNAL(done()), &m_done_mapper, SLOT(map()));
        m_done_mapper.setMapping(m_managers[i], i);
        connect(m_managers[i], SIGNAL(selected()), &m_select_mapper, SLOT(map()));
        m_select_mapper.setMapping(m_managers[i], i);
    }
    
    connect(&m_done_mapper, SIGNAL(mapped(int)), this, SLOT(chosen(int)));
    connect(&m_select_mapper, SIGNAL(mapped(int)), this, SLOT(selected(int)));
}

class AddEntityVisitor : public OptionVisitor
{
    GeneralController* m_controller;
    Sea::Player m_player;
    SeaView* m_sea;
    ChatWidget* m_chat;
    
public:
    AddEntityVisitor(GeneralController* controller, Sea::Player player,
                     SeaView* sea, ChatWidget* chat)
    : m_controller(controller)
    , m_player(player)
    , m_sea(sea)
    , m_chat(chat)
    {
    }
    
    virtual void visit(const HumanChooserOption& option)
    {
        PlayerEntity* entity = m_controller->createPlayer(
            m_player, m_sea, m_chat, option.nick());
        
        QObject::connect(m_chat, SIGNAL(message(QString, QString)),
            entity, SIGNAL(chat(QString, QString)));
    }
    
    virtual void visit(const AIChooserOption& option)
    {
        m_controller->createAI(m_player);
        // TODO: use the chosen difficulty level
        Q_UNUSED(option);
    }
    
    virtual void visit(const NetworkChooserOption& option)
    {
        QTcpSocket* socket = option.socket();
        m_controller->createRemotePlayer(m_player, socket, option.server());

        m_chat->show();
    }
};

class OnSelectedVisitor : public OptionVisitor
{
    Sea::Player m_player;
    ScreenManager** m_managers;
public:
    OnSelectedVisitor(Sea::Player player, ScreenManager** managers)
    : m_player(player)
    , m_managers(managers)
    {
    }

    virtual void visit(const HumanChooserOption&)
    {
        m_managers[Sea::opponent(m_player)]->removeHumanButton();
    }
    
    virtual void visit(const AIChooserOption&)
    {
    }
    
    virtual void visit(const NetworkChooserOption&)
    {
    }
};


void GameChooser::selected(int player)
{
    ChooserOption* option = m_managers[player]->option();
    Q_ASSERT(option);
    
    OnSelectedVisitor visitor(Sea::Player(player), m_managers);
    option->apply(visitor);
}

void GameChooser::chosen(int player)
{
    ChooserOption* option = m_managers[player]->option();
    Q_ASSERT(option);
//     PrintOption visitor;
    kDebug() << "player " << player << " has chosen" << endl;
//     m_managers[player]->option()->apply(visitor);

    if (complete()) {
        kDebug() << "emitting GameChooser::done()" << endl;
        emit done();
    }
}

bool GameChooser::complete() const
{
    return m_managers[0]->option() &&
           m_managers[0]->option()->complete() &&
           m_managers[1]->option() &&
           m_managers[1]->option()->complete();
}

void GameChooser::setupController(GeneralController* controller, SeaView* sea, ChatWidget* chat)
{
    Q_ASSERT(complete());
    
    for (int i = 0; i < 2; i++) {
        chat->hide();
        
        AddEntityVisitor visitor(controller, Sea::Player(i), sea, chat);
        m_managers[i]->option()->apply(visitor);
    }
    
    controller->start(sea);
}

#include "gamechooser.moc"
