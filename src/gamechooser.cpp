#include "gamechooser.h"

#include <kdebug.h>
#include <klocale.h>
#include <kicon.h>
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
{
}

void HumanChooserOption::initialize()
{
    m_screen->fadeOut();
    emit done();
}


void HumanChooserOption::apply(OptionVisitor& visitor)
{
    visitor.visit(*this);
}

AIChooserOption::AIChooserOption(WelcomeScreen* screen)
: m_screen(screen)
, m_level(Easy)
{
}

void AIChooserOption::initialize()
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

void NetworkChooserOption::initialize()
{
    m_screen->clearButtons();
    Button* button;
     
    button = m_screen->addButton(0, 0, QIcon(), i18n("Server"));
    connect(button, SIGNAL(clicked()), this, SLOT(setServer()));
    
    button = m_screen->addButton(0, 1, QIcon(), i18n("Client"));
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
    button = screen->addButton(0, 0, KIcon("user-female"), i18n("Human"));
    connect(button, SIGNAL(clicked()), this, SLOT(human()));
    
    button = screen->addButton(0, 1, KIcon("roll"), i18n("Computer"));
    connect(button, SIGNAL(clicked()), this, SLOT(ai()));
    
    button = screen->addButton(0, 2, KIcon("network"), i18n("Network"));
    connect(button, SIGNAL(clicked()), this, SLOT(network()));
}

void ScreenManager::human()
{
    setOption(new HumanChooserOption(m_screen));
}

void ScreenManager::ai()
{
    setOption(new AIChooserOption(m_screen));
}

void ScreenManager::network()
{
    setOption(new NetworkChooserOption(m_screen));
}

void ScreenManager::setOption(ChooserOption* option)
{
    if (!m_option) {
        m_option = option;
        connect(m_option, SIGNAL(done()), this, SIGNAL(done()));
        kDebug() << "initializing option" << endl;
        option->initialize();
    }
}

GameChooser::GameChooser(QObject* parent, WelcomeScreen* screen0, WelcomeScreen* screen1)
: QObject(parent)
{
    m_managers[0] = new ScreenManager(this, screen0);
    m_managers[1] = new ScreenManager(this, screen1);
    
    connect(m_managers[0], SIGNAL(done()), &m_mapper, SLOT(map()));
    connect(m_managers[1], SIGNAL(done()), &m_mapper, SLOT(map()));
    m_mapper.setMapping(m_managers[0], 0);
    m_mapper.setMapping(m_managers[1], 1);
    
    connect(&m_mapper, SIGNAL(mapped(int)), this, SLOT(choose(int)));
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


void GameChooser::choose(int player)
{
    Q_ASSERT(m_managers[player]->option());
//     PrintOption visitor;
    kDebug() << "player " << player << " has chosen" << endl;
//     m_managers[player]->option()->apply(visitor);

    if (complete()) {
        emit done();
    }
}

bool GameChooser::complete() const
{
    return m_managers[0]->option() && m_managers[1]->option();
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
