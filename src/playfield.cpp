/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "playfield.h"

#include <QStatusBar>
#include <QVBoxLayout>
#include <QDebug>
#include <QInputDialog>

#include <KLocalizedString>
#include <KMessageBox>
#include <KScoreDialog>

#include "aientity.h"
#include "audioplayer.h"
#include "chatwidget.h"
#include "controller.h"
#include "playerentity.h"
#include "seaview.h"
#include "settings.h"
#include "simplemenu.h"
#include "stats.h"
#include "welcomescreen.h"

static const int MINIMUM_HEIGHT = 400;

PlayField::PlayField(QWidget* parent, QStatusBar* sbar)
: QWidget(parent)
, m_status_bar(sbar)
, m_show_endofgame_message(true)
{
    setMinimumSize(static_cast<int>(MINIMUM_HEIGHT * 1.6), MINIMUM_HEIGHT);
    QVBoxLayout* layout = new QVBoxLayout;
    
    m_seaView = new SeaView(this);
    layout->addWidget(m_seaView, 1);
    
    m_chat = new ChatWidget(this);
    m_chat->hide();
    layout->addWidget(m_chat, 1);
    
    layout->setMargin(0);
//     layout->setSpacing(0);
    setLayout(layout);
        
    m_controller = 0;
    m_menu = 0;
    
    m_player = new AudioPlayer(this);
    m_player->setActive(Settings::enableSounds());
}

PlayField::~PlayField()
{
    // controller assumes that the view is still valid
    // when it is destroyed
    delete m_controller;
}

Controller* PlayField::createController()
{
    // The client or server will overwrite this default configuration when 
    // the network messages are interchanged
    m_battle_ships_configuration = Settings::severalShips() ? 
    BattleShipsConfiguration::defaultMultipleShipsConfiguration(Settings::adjacentShips()):
    BattleShipsConfiguration::defaultSingleShipsConfiguration(Settings::adjacentShips());
    Controller* controller = new Controller(this, m_player, m_battle_ships_configuration);
    connect(controller, &Controller::gameOver, this, &PlayField::gameOver);
    connect(controller, &Controller::restartRequested, this, &PlayField::restartRequested);
    connect(controller, &Controller::compatibility, this, &PlayField::setCompatibility);
    connect(controller, &Controller::nickChanged, this, &PlayField::updateNick);
    connect(controller, &Controller::turnChanged, this, &PlayField::changeTurn);
    connect(controller, &Controller::playerReady, this, &PlayField::playerReady);
    connect(controller, &Controller::restartPlacingShips, this, &PlayField::restartPlacingShips);
    connect(controller, &Controller::startPlacingShips, this, &PlayField::startPlacingShips);

    return controller;
}

void PlayField::setupController()
{
    Animator::instance()->restart();
    m_seaView->clear();
    m_chat->hide();
    
    // remove welcome screen
    m_seaView->screen(Sea::Player(0))->fadeOut();
    m_seaView->screen(Sea::Player(1))->fadeOut();

    delete m_controller;
    m_controller = createController();
    m_menu->setupController(m_controller, 0, m_seaView, m_chat, false);
    startGame();
}

void PlayField::endGame()
{
    Animator::instance()->restart();
    delete m_controller;
    m_controller = 0;
    m_seaView->clear();
}

void PlayField::newGame()
{
    endGame();
    delete m_menu;
    
    Kg::difficulty()->setGameRunning(false);
    
    m_chat->hide();
    m_seaView->screen(Sea::Player(0))->show();
    m_seaView->screen(Sea::Player(1))->show();
    
    m_menu = new SimpleMenu(this, m_seaView->screen(Sea::Player(0)));
    connect(m_menu, &SimpleMenu::done, this, &PlayField::setupController);
    m_status_bar->showMessage(QLatin1String(""));
    emit welcomeScreen();
}

void PlayField::restart(bool ask)
{
    Animator::instance()->restart();
    m_seaView->clear();
    startGame();
    m_controller->restart(ask);
}


void PlayField::highscores()
{
    KScoreDialog* highscoredialog = new KScoreDialog(
            KScoreDialog::Name | KScoreDialog::Score | 
            KScoreDialog::Custom1 | 
            KScoreDialog::Custom2 | 
            KScoreDialog::Custom3, 
            this);
    highscoredialog->initFromDifficulty(Kg::difficulty());
    highscoredialog->addField(KScoreDialog::Custom1, i18n("Shots"), QLatin1Literal("shots"));
    highscoredialog->addField(KScoreDialog::Custom2, i18n("Hits"), QLatin1Literal("hits"));
    highscoredialog->addField(KScoreDialog::Custom3, i18n("Misses"), QLatin1Literal("water"));
    
    highscoredialog->exec();
}

void PlayField::gameOver(Sea::Player winner)
{
    if (winner == Sea::Player(0)) {
        const Stats* stats = m_menu->player(0)->stats();
       
        if (stats && qobject_cast<const AIEntity*>(m_menu->player(1))) {
            KScoreDialog* highscoredialog = new KScoreDialog(
                    KScoreDialog::Name | KScoreDialog::Score | 
                    KScoreDialog::Custom1 | 
                    KScoreDialog::Custom2 | 
                    KScoreDialog::Custom3, 
                    this);
            highscoredialog->initFromDifficulty(Kg::difficulty());
            highscoredialog->addField(KScoreDialog::Custom1, i18n("Shots"), QLatin1Literal("shots"));
            highscoredialog->addField(KScoreDialog::Custom2, i18n("Hits"), QLatin1Literal("hits"));
            highscoredialog->addField(KScoreDialog::Custom3, i18n("Misses"), QLatin1Literal("water"));
        
            KScoreDialog::FieldInfo info;
            info[KScoreDialog::Name] = m_menu->player(0)->nick();
            info[KScoreDialog::Score].setNum(stats->score());
            info[KScoreDialog::Custom1] = QString::number(stats->shots());
            info[KScoreDialog::Custom2] = QString::number(stats->hits());
            info[KScoreDialog::Custom3] = QString::number(stats->misses());
        
            int temp = highscoredialog->addScore(info, KScoreDialog::AskName);
            qDebug() << "temp =" << temp;
            //if (highscoredialog->addScore(info, KScoreDialog::AskName)) {
            if (temp != 0) {
                highscoredialog->exec();
                return;
            }
        }
        
        m_status_bar->showMessage(i18n("You win!"));
        if (m_show_endofgame_message) {
            KMessageBox::information(this, i18n("You win. Excellent!"));
        }
    }
    else {
        m_status_bar->showMessage(i18n("You lose."));
        if (m_show_endofgame_message) {
            KMessageBox::information(this, i18n("You lose. Better luck next time!"));
        }
    }
    
    // When we have finished, show again the welcome screen
    emit gameFinished();
}

void PlayField::changeNick()
{
    QString nick = QInputDialog::getText(this, i18n("Change Nickname"), i18n("Enter new nickname:"), QLineEdit::Normal, Settings::findNick());
    if (!nick.isEmpty()) {
        Settings::setNickname(nick);
        Settings::self()->save();
    }
}

void PlayField::toggleSounds(bool enable)
{
    Settings::setEnableSounds(enable);
    Settings::self()->save();
    m_player->setActive(enable);
}

void PlayField::toggleAdjacent(bool enable)
{
    Settings::setAdjacentShips(enable);
    Settings::self()->save();
}

void PlayField::toggleMultiple(bool enable)
{
    Settings::setSeveralShips(enable);
    Settings::self()->save();
}

void PlayField::restartRequested()
{
    int ans = KMessageBox::questionYesNo(this, i18n("Restart game"),
                    i18n("Your opponent has requested to restart the game. Do you accept?"));
    if (ans == KMessageBox::Yes) {
        restart(false);
    }
}

void PlayField::setCompatibility(int level)
{
    if (level == Entity::COMPAT_KBS3) {
        KMessageBox::information(this, i18n("Your opponent is using a pre-KDE4 version of Naval Battle. Note that, according to the rules enforced by old clients, ships cannot be placed adjacent to one another and only one ship of each size is allowed."));
    }
}

void PlayField::updateNick(int player, const QString& nick)
{
    m_seaView->setNick(Sea::Player(player), nick);
}

void PlayField::changeTurn(int player)
{
    if (player == 0) {
        // local user
        m_status_bar->showMessage(i18n("Enemy has shot. Shoot now!"));
    }
    else {
        // opponent
        m_status_bar->showMessage(i18n("Waiting for enemy to shoot..."));
    }
}

void PlayField::playerReady(int player)
{
    if (player == -1) {
        // game can start
        if (m_controller->turn() == 0) {
            m_status_bar->showMessage(i18n("Ships placed. Now shoot on the enemy field!"));
        }
        else {
            m_status_bar->showMessage(i18n("Waiting for other player to start the game..."));
        }
    }
    else if (player == 0) {
        m_status_bar->showMessage(i18n("Waiting for other player to place his ships..."));
    }
}

void PlayField::startGame()
{
    //the game has a fixed difficulty level only if there is an AI
    Kg::difficulty()->setGameRunning(m_controller->hasAI());
    startPlacingShips();
    emit placeShips();
}

void PlayField::startPlacingShips()
{
    m_status_bar->showMessage(i18n("Place your %1 ships. Use the right mouse button to rotate them.", m_battle_ships_configuration.totalNumberOfShipsToPlay()));
}


void PlayField::restartPlacingShips(Sea::Player player)
{
    m_status_bar->showMessage(i18n("You can't place your remaining ships."));
    KGuiItem buttonRestart=KStandardGuiItem::yes(); buttonRestart.setText(i18n("Restart"));
    KGuiItem buttonAbort=KStandardGuiItem::no(); buttonAbort.setText(i18n("Abort"));
    int res=KMessageBox::warningYesNo(this, i18n("You can't place your remaining ships. Please restart placing ships or abort game"), i18n("Restart placing ships"), buttonRestart, buttonAbort);
    if (res == KMessageBox::Yes) {
        startPlacingShips();
        m_controller->notifyRestartPlacingShips(player);
    }
    else {
        newGame();
    }
}


void PlayField::levelChanged()
{
    if (m_controller && m_controller->hasAI()) {
        restart();
    }
}

SimpleMenu* PlayField::createAuxMenu()
{
    SimpleMenu* menu = new SimpleMenu(this, 0);
    connect(menu, &SimpleMenu::done, this, &PlayField::auxMenuDone);
    return menu;
}

void PlayField::auxMenuDone()
{
    qDebug() << "aux menu done";
    SimpleMenu* menu = qobject_cast<SimpleMenu*>(sender());
    if (menu) {
        delete m_menu;
        m_menu = menu;
        setupController();
    }
}

void PlayField::localGame()
{
    createAuxMenu()->localGame();
}

void PlayField::createServer()
{
    createAuxMenu()->createServer();
}

void PlayField::createClient()
{
    createAuxMenu()->createClient();
}

void PlayField::createClientWithUrl(const QUrl& url)
{
    createAuxMenu()->createClientWithUrl(url);
}

void PlayField::toggleEndOfGameMessage(bool show)
{
    m_show_endofgame_message = show;
}

void PlayField::toggleLeftGrid(bool show)
{
    m_seaView->toggleLeftGrid(show);
}

void PlayField::toggleRightGrid(bool show)
{
    m_seaView->toggleRightGrid(show);
}        




