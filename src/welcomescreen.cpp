/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "welcomescreen.h"

#include <kdebug.h>
#include <kicon.h>
#include <klocale.h>

#include "button.h"
#include "animator.h"

WelcomeScreen::WelcomeScreen(KGameCanvasAbstract* parent, const QFont& font)
: KGameCanvasGroup(parent)
, m_font(font)
, m_clicked(0)
, m_hover(0)
, m_active(true)
{
    m_background = new KGameCanvasRectangle(QColor(0, 0, 0, 80), m_size, this);
    m_background->show();
}

void WelcomeScreen::resize(const QSize& size)
{
    m_size = size;
    
    // background
    m_background->setSize(m_size);
    m_background->moveTo(0, 0);
    
    update();
}

void WelcomeScreen::update()
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
    QSize size = m_size;
    size.rwidth() /= max_x;
    size.rheight() /= max_y;
    
    for (Buttons::const_iterator i = m_buttons.constBegin();
         i != m_buttons.constEnd();
         ++i) {
        QPoint pos(size.width() * i.key().x,
                   size.height() * i.key().y);
        QPoint delta((size.width() - i.value()->size().width()) / 2,
                     (size.height() - i.value()->size().height()) / 2);
        i.value()->moveTo(pos + delta);
        i.value()->repaint();
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
        button->show();
        update();
        connect(button, SIGNAL(needsUpdate()), this, SLOT(update()));
        
        kDebug() << "added button" << button;
        
        return button;
    }
}

void WelcomeScreen::removeButton(int x, int y)
{
    Button* button = m_buttons.take(Coord(x, y));
    delete button;
    update();
}

void WelcomeScreen::moveButton(int x1, int y1, int x2, int y2)
{
    Coord from(x1, y1);
    Coord to(x2, y2);
    
    if (m_buttons.contains(from) && !m_buttons.contains(to)) {
        Button* button = m_buttons.value(from);
        m_buttons.insert(to, button);
        m_buttons.remove(from);
        update();
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

void WelcomeScreen::onMouseMove(const QPoint& p)
{
    Button* button = dynamic_cast<Button*>(itemAt(p));
//     kDebug() << "on point" << p << "there is" << button;
    if (m_hover && m_hover != button) {
        m_hover->onMouseLeave();
    }

    m_hover = button;
    if (button && (!m_clicked || button == m_clicked)) {
        button->onMouseMove(p - button->pos());
    }
}

void WelcomeScreen::onMousePress(const QPoint& p)
{
    kDebug() << "on mouse press";
    Button* button = dynamic_cast<Button*>(itemAt(p));
    if (button) {
        button->onMousePress(p - button->pos());
        m_clicked = button;
    }
}

void WelcomeScreen::onMouseRelease(const QPoint& p)
{
    if (m_clicked) {
        m_clicked->onMouseRelease(p - m_clicked->pos());
    }

    Button* button = dynamic_cast<Button*>(itemAt(p));
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
    Animation* hideAnimation = new FadeAnimation(this, opacity(), 0, 500);
    connect(hideAnimation, SIGNAL(done()), this, SLOT(hide()));
    Animator::instance()->add(hideAnimation);
}

void WelcomeScreen::show()
{
    m_active = true;
    setOpacity(255);
    KGameCanvasGroup::show();
    emit shown();
}

void WelcomeScreen::hide()
{
    m_active = false;
    KGameCanvasGroup::hide();
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

