#include "gamechooser.h"

#include <kdebug.h>
#include <klocale.h>
#include <kicon.h>
#include <QLineEdit>
#include "button.h"
#include "chatwidget.h"
#include "generalcontroller.h"
#include "sea.h"

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
    m_button->setEditor(config.readEntry("nickname"));
    QLineEdit* editor = button->editor();
    Q_ASSERT(editor);
    connect(editor, SIGNAL(editingFinished()), this, SLOT(finalize()));

}

void HumanChooserOption::finalize()
{
    Q_ASSERT(m_button);
    
    QLineEdit* editor = m_button->editor();
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
, m_port(54321)
{
}

void NetworkChooserOption::initialize(Button*)
{
    m_screen->clearButtons();
    Button* button;
     
    button = m_screen->addButton(0, 0, QIcon(), i18n("Wait for a connection"));
    connect(button, SIGNAL(clicked()), this, SLOT(setServer()));
    
    button = m_screen->addButton(0, 1, QIcon(), i18n("Connect to a server"));
    connect(button, SIGNAL(clicked()), this, SLOT(setClient()));
}

void NetworkChooserOption::apply(OptionVisitor& visitor)
{
    visitor.visit(*this);
}

void NetworkChooserOption::setServer()
{
    m_server = true;
    finalize();
}

void NetworkChooserOption::setClient()
{
    m_server = false;
    m_host = "localhost";
    finalize();
}

void NetworkChooserOption::setHost(const QString& host)
{
    m_host = host;
}

void NetworkChooserOption::setPort(const QString& port)
{
    m_port = port.toInt();
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
        m_controller->createPlayer(m_player, m_sea, m_chat, option.nick());
    }
    
    virtual void visit(const AIChooserOption& option)
    {
        m_controller->createAI(m_player);
        // TODO: use the chosen difficulty level
        Q_UNUSED(option);
    }
    
    virtual void visit(const NetworkChooserOption& option)
    {
        kDebug() << "network option not implemented" << endl;
//         m_chat->show(); //TODO: decomment me
        Q_UNUSED(option);
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
