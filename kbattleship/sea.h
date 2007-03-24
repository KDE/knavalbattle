#ifndef Sea_H
#define Sea_H

#include <QObject>
#include "ship.h"
#include "hitinfo.h"

class BattleField;

class Sea : public QObject
{
Q_OBJECT
public:
    enum Status
    {
        PLACING_SHIPS,
        PLAYING,
        A_WINS,
        B_WINS
    };
    enum Player
    {
        PLAYER_A = 0,
        PLAYER_B = 1
    };
private:
    Coord m_size;
    Player m_turn;
    BattleField* m_fields[2];
    Status m_status;
    
    void switchTurn();
    inline BattleField* currentField() const { return m_fields[m_turn]; }
    inline BattleField* otherField() const { return m_fields[opponent(m_turn)]; }
public:
    Sea(QObject* parent, const Coord& size);
    
    bool canAddShip(Player p, const Coord& pos, int size, Ship::Direction direction) const;
    void add(Player p, const Coord& pos, Ship* ship);
    bool canHit(Player p, const Coord& pos) const;
    HitInfo hit(const Coord& pos);
    void startPlaying();
    
    inline Status status() const { return m_status; }
    inline Player turn() const { return m_turn; }
    static Player opponent(Player p);
};

#endif // Sea_H

