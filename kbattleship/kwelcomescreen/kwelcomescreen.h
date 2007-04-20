/******************************************************************************
*   Copyright (C) 2007 by Riccardo Iaconelli  <ruphy@fsfe.org>                *
*                                                                             *
*   This library is free software; you can redistribute it and/or             *
*   modify it under the terms of the GNU Library General Public               *
*   License as published by the Free Software Foundation; either              *
*   version 2 of the License, or (at your option) any later version.          *
*                                                                             *
*   This library is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
*   Library General Public License for more details.                          *
*                                                                             *
*   You should have received a copy of the GNU Library General Public License *
*   along with this library; see the file COPYING.LIB.  If not, write to      *
*   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
*   Boston, MA 02110-1301, USA.                                               *
*******************************************************************************/

#ifndef KWELCOMESCREEN_H
#define KWELCOMESCREEN_H

#include <QAbstractButton>
#include <kdemacros.h>

class QPaintEvent;
class QGridLayout;
class KWelcomeWidget;
class KWelcomeScreenButton;
class QEvent;
class QSize;
class QGridLayout;
class QResizeEvent;
class KWelcomeScreenOverlayButton;

class KDE_EXPORT KWelcomeScreenOverlay
{
    friend class KWelcomeScreenOverlayButton;
protected:
    QSize m_size;
    QVector<KWelcomeScreenOverlayButton *> m_buttons;
    QColor m_background_color;
    QFont m_font;
    KWelcomeScreenOverlayButton *m_hover_button;

    virtual KWelcomeScreenOverlayButton *createButton(const QString &text, const QIcon &icon);
    virtual KWelcomeScreenOverlayButton *at(QPoint& pos);
    virtual void invalidateScreen(const QRect& rect) = 0;
    virtual void buttonClicked(KWelcomeScreenOverlayButton* button) = 0;
    
    virtual void invalidateButton(KWelcomeScreenOverlayButton* button);
public:
    explicit KWelcomeScreenOverlay(const QFont&);
    
    virtual ~KWelcomeScreenOverlay();
    virtual void drawScreen(QPainter& p);
    virtual void addButton(const QString &text, const QIcon &icon);
    virtual void resize(const QSize& sz);
    virtual void onMousePress(const QPoint& p);
    virtual void onMouseRelease(const QPoint& p);
    virtual void onMouseMove(const QPoint& p);
    virtual void onMouseLeave();
    
    void setBackgroundColor(const QColor &color) { m_background_color = color; }
    QColor backgroundColor() const { return m_background_color; }
    QSize size() const { return m_size; }
};

class KDE_EXPORT KWelcomeScreenOverlayButton
{
    KWelcomeScreenOverlay *m_parent;
    QString m_text;
    QIcon m_icon;
    QRect m_rect;
    enum State {
        UP,
        DOWN,
        HOVER
    } m_state;
    bool m_enabled;
    double m_opacity;
public:
    KWelcomeScreenOverlayButton(KWelcomeScreenOverlay *parent, const QFont& font, 
                                const QString &text, const QIcon &icon);
    virtual ~KWelcomeScreenOverlayButton() { }
    
    virtual void drawButton(QPainter& p);
    virtual void onMousePress(const QPoint& p);
    virtual void onMouseMove(const QPoint& p);
    virtual void onMouseRelease(const QPoint& p);
    virtual void onMouseLeave();
    
    const QRect& rect() const { return m_rect; }
    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool value);
    double opacity() const { return m_opacity; }
    void setOpacity(double value);
};

class KDE_EXPORT KWelcomeScreen : public QWidget
{
    Q_OBJECT
public:
    explicit KWelcomeScreen(const QString& name, QWidget *parent = 0);

    void addButton(const QString &text, const QIcon &icon);
public Q_SLOTS:
    void hideOverlay();
    void showOverlay();
    void init();
    
private:
    KWelcomeWidget *widget;
    QGridLayout *mainLayout;
    QString m_applicationName;

signals:
    void bluttonClicked(int);
};

class KWelcomeWidget : public QWidget, public KWelcomeScreenOverlay
{
    Q_OBJECT
public:
    explicit KWelcomeWidget(const QString& name, QWidget *parent = 0);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void invalidateScreen(const QRect &rect);
    virtual void buttonClicked(KWelcomeScreenOverlayButton *button);
private:
    QGridLayout *layout;
    QWidget *frame;
    QVector<KWelcomeScreenButton *> buttons;
    QString m_applicationName;
//     KWelcomeScreenPrivate *d;
signals:
    void clicked(int);
};

#endif
