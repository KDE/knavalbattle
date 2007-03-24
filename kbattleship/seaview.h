#ifndef SEA_H
#define SEA_H

#include <kgamecanvas.h>
#include "sea.h"

class BattleFieldView;
class KBSRenderer;
class Controller;

class SeaView : public KGameCanvasWidget
{
    static const unsigned int GRID_SIZE = 10;
    BattleFieldView* m_fields[2];
    
    KBSRenderer* m_renderer;
    Controller* m_controller;
    BattleFieldView* otherField(BattleFieldView* field);
    
    void setPreview(Sea::Player player, const QPoint& pos);
    int fieldAt(const QPoint& p);
public:
    SeaView(QWidget* parent);
    
    void update();
    void setController(Controller* c);
    void cancelPreview();
    void add(Sea::Player p, const Coord& c, Ship* ship);
    void hit(Sea::Player p, const Coord& c);
    void miss(Sea::Player p, const Coord& c);
    void gameOver(Sea::Status status);
protected:
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
    
    int tileSize() const;
};

#endif // SEA_H

