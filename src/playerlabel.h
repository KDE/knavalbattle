#ifndef PLAYERLABEL_H
#define PLAYERLABEL_H

#include <KGameCanvas>

class PlayerLabel : public KGameCanvasGroup
{
public:
    static const int HEIGHT = 32;
    static const int MARGIN = 10;
    
private:
    KGameCanvasPixmap* m_icon;
    KGameCanvasText* m_name;
    
public:
    PlayerLabel(const QPixmap& icon, const QString& text, KGameCanvasAbstract* parent);

    void update();
    int height() const;
};

#endif // PLAYERLABEL_H

