/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "welcomescreen.h"

#include <QDebug>

#include <kicon.h>
#include <klocale.h>

#include "button.h"
#include "animator.h"

WelcomeScreen::WelcomeScreen(const QFont& font)
: QObject()
, QGraphicsRectItem()
, m_font(font)
, m_clicked(0)
, m_hover(0)
, m_active(true)
{
    QBrush brush(QColor(0, 0, 0, 80));
    setBrush(brush);
}

void WelcomeScreen::resize(const QSizeF& size)
{
    m_size = size;
    
    // background
    QRectF rect(QPoint(0, 0), m_size);
    setRect(rect);
    
    refresh();
}

void WelcomeScreen::refresh()
{
    // find dimensions
    int max_x = 0;
    int max_y = 0;
    for (Buttons::const_iterator i = m_buttons.constBegin();
         i != m_buttons.constEnd();
         ++i) {
        if (i.key().x > max_x) {
            max_x = i.key().x;
        }
        if (i.key().y > max_y) {
            max_y = i.key().y;
        }    
    }
    max_x++;
    max_y++;
    
    // place buttons
    QSizeF size = m_size;
    size.rwidth() /= max_x;
    size.rheight() /= max_y;
    
    for (Buttons::const_iterator i = m_buttons.constBegin();
         i != m_buttons.constEnd();
         ++i) {
        QPoint pos(size.width() * i.key().x,
                   size.height() * i.key().y);
        QPoint delta((size.width() - i.value()->size().width()) / 2,
                     (size.height() - i.value()->size().height()) / 2);
        i.value()->setPos(pos + delta);
        i.value()->update();
    }
}

Button* WelcomeScreen::addButton(int x, int y, const QIcon& icon, const QString& text)
{
    if (m_buttons.contains(Coord(x, y))) {
        return m_buttons[Coord(x, y)];
    }
    else {
        Button* button = new Button(this, icon, m_font, text);
        if (!m_buttons.isEmpty()) {
            Button* other = *m_buttons.begin();
            if (other->size().width() >= button->size().width()) {
                button->setWidth(other->size().width());
            }
            else {
                for (Buttons::const_iterator i = m_buttons.constBegin();
                     i != m_buttons.constEnd(); ++i) {
                    (*i)->setWidth(button->size().width());
                }
            }
        }
        m_buttons.insert(Coord(x, y), button);
        refresh();
        connect(button, SIGNAL(needsUpdate()), this, SLOT(refresh()));
        
        //qDebug() << "added button" << button;
        
        return button;
    }
}

void WelcomeScreen::removeButton(int x, int y)
{
    Button* button = m_buttons.take(Coord(x, y));
    delete button;
    refresh();
}

void WelcomeScreen::moveButton(int x1, int y1, int x2, int y2)
{
    Coord from(x1, y1);
    Coord to(x2, y2);
    
    if (m_buttons.contains(from) && !m_buttons.contains(to)) {
        Button* button = m_buttons.value(from);
        m_buttons.insert(to, button);
        m_buttons.remove(from);
        refresh();
    }
}

void WelcomeScreen::clearButtons()
{
    m_clicked = 0;
    m_hover = 0;
    for (Buttons::const_iterator i = m_buttons.constBegin();
         i != m_buttons.constEnd();
         ++i) {
        delete *i;
    }
    m_buttons.clear();
    
}

void WelcomeScreen::onMouseMove(Button *button)
{
    if (m_hover && m_hover != button) {
        m_hover->onMouseLeave();
    }

    m_hover = button;
    if (!m_clicked || button == m_clicked) {
        button->onMouseMove();
    }
}

void WelcomeScreen::onMousePress(Button *button)
{
    qDebug() << "on mouse press";

    button->onMousePress();
    m_clicked = button;
}

void WelcomeScreen::onMouseRelease(Button *button)
{
    if (m_clicked) {
        m_clicked->onMouseRelease();
    }

    if (m_clicked && m_clicked == button) {
        // actual click event
        if (m_clicked->onClicked()) {            
            emit clicked(button);
        }
    }
    
    m_clicked = 0;
}

void WelcomeScreen::fadeOut()
{
    Animation* hideAnimation = new FadeAnimation(this, 1, 0, 500);
    connect(hideAnimation, SIGNAL(done()), this, SLOT(hide()));
    Animator::instance()->add(hideAnimation);
}

void WelcomeScreen::show()
{
    m_active = true;
    setOpacity(1);
    QGraphicsRectItem::show();
    emit shown();
}

void WelcomeScreen::hide()
{
    m_active = false;
    QGraphicsRectItem::hide();
    clearButtons();
    emit hidden();
}

void WelcomeScreen::onMouseLeave()
{
    if (m_hover) {
        m_hover->onMouseLeave();
    }
}



#include "welcomescreen.moc"

