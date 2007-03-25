#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "sea.h"

class SeaView;

class Controller : public QObject
{
Q_OBJECT
public:
    Controller(QObject* parent);
    virtual ~Controller() { }
    virtual Ship* canAddShip(Sea::Player, const Coord& c) = 0;
    virtual void action(Sea::Player, const Coord& c) = 0;
    virtual void changeDirection(Sea::Player) = 0;
};

class OnePlayerController : public Controller
{
Q_OBJECT
    QList<Ship*> m_ships;
    Sea* m_sea;
    SeaView* m_view;
    Sea::Player m_player;
    
    void hit(Sea::Player player, const Coord& c);
public:
    OnePlayerController(QObject* parent, SeaView* view, Sea::Player player);
    
    virtual Ship* canAddShip(Sea::Player, const Coord& c);
    virtual void action(Sea::Player, const Coord& c);
    virtual void changeDirection(Sea::Player);
    Ship* nextShip();
};

#endif // CONTROLLER_H

