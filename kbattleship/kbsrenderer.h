#ifndef KBSRENDERER_H
#define KBSRENDERER_H

#include <QString>
#include <QPixmap>
#include <QHash>

class KSvgRenderer;

/**
  * Class to render KBattleShip graphical elements.
  */
class KBSRenderer
{
    typedef QHash<QString, QPixmap> Cache; // use QCache maybe?
public:
    /**
      * Create a new renderer instance. Each instance has a different cache.
      */
    KBSRenderer(const QString& path);

    ~KBSRenderer();

    /**
      * Set a new size for the elements. Invalidate cache.
      */
    void resize(int sz);
    void resize(const QSize& sz);

    /**
      * Return current pixmap size.
      */
    QSize size() const;

    /**
      * Set a new size for the background. Invalidate background cache.
      */
    void resizeBackground(const QSize& sz);

    /**
      * Render an item ensuring it is in the cache.
      */
    QPixmap render(const QString& id);
    QPixmap render(int type, bool rotate = false);

    /**
      * Render background.
      */
    QPixmap renderBackground();
protected:
    QPixmap render(const QString& id, const QSize& sz);
private:
    KSvgRenderer* m_renderer;
    QSize m_size;
    QSize m_background_size;

    Cache m_cache;
    QHash<int, QString> m_typenames;
    QPixmap m_background;
};

#endif // KBSRENDERER_H
