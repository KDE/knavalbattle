#ifndef KBATTLESHIPFIELD_H
#define KBATTLESHIPFIELD_H

#include <kgamecanvas.h>
#include <kgrid2d.h>
#include <kstandarddirs.h>

class KBSRenderer;

class ManagedGroup : public KGameCanvasGroup
{
public:
    ManagedGroup(KGameCanvasAbstract* parent)
            : KGameCanvasGroup(parent)
    { }
    void clear()
    {
        while (!items()->isEmpty())
            delete items()->first();
    }
    ~ManagedGroup()
    {
        clear();
    }
};

class KFieldItem
{
public:
    KFieldItem(int type = 0, KGameCanvasItem* sprite = 0)
            : m_type(type)
            , m_sprite(sprite)
    { }

    int type() const
    {
        return m_type;
    }
    KGameCanvasItem* sprite() const
    {
        return m_sprite;
    }
    void setSprite(KGameCanvasItem* sprite)
    {
        m_sprite = sprite;
    }
private:
    int m_type;
    KGameCanvasItem *m_sprite;
};

class KBattleShipField : public ManagedGroup
{
public:
    typedef KGrid2D::Coord Coord;
    typedef KGrid2D::Generic<KFieldItem> Grid;

    KBattleShipField(KGameCanvasAbstract* parent, KBSRenderer* renderer, int size);

    Coord getCoord(const QPoint& p) const;
    void clear();
    int state(int x, int y) const;
    void setState(int x, int y, int type);
    void setPreviewState(int x, int y, int type, bool rotate);
    int gridSize() const;
private:
    KBSRenderer* m_renderer;
    int m_size;
    Grid m_grid;

    KGameCanvasPixmap* m_background;
    KGameCanvasItem* m_preview;

    KGameCanvasGroup* createShip(int x, int y, int type, bool rotate);
    KGameCanvasPixmap* createSprite(const Coord&, int type, bool rotate, KGameCanvasAbstract* parent = 0);
    void incCoord(Coord& p, bool rotate) const;
    bool valid(const Coord& p) const;
};

#endif // KBATTLESHIPFIELD_H
