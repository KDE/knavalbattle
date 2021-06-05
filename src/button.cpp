/*
  Copyright (c) 2007 Riccardo Iaconelli <ruphy@fsfe.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "button.h"

#include <QPainter>
#include <QPen>
#include <QImage>
#include "knavalbattle_debug.h"

#include <math.h> // fabs

#include "animator.h"

Button::Button(QGraphicsItem* parent, const QIcon& icon, 
               const QFont& font, const QString& text)
: QGraphicsObject(parent)
, m_icon(icon)
, m_font(font)
, m_text(text)
, m_fixed_width(false)
, m_down(false)
, m_hover(false)
, m_brightness(BRIGHTNESS_NORMAL)
, m_editor(nullptr)
{
    computeSize();
}

Button::~Button()
{
    if (m_animation) {
        m_animation->abort();
    }
    //delete m_editor;
}

void Button::setWidth(int width)
{
    m_fixed_width = width != -1;
    m_size.setWidth(width);
    computeSize();
}

void Button::computeSize()
{
    QFontMetrics fm(m_font);
    m_text_width = fm.boundingRect(m_text).width();
    int h = fm.height();
    if (h < 32) {
        h = 32;
    }
    if (!m_fixed_width) {        
        m_size = QSize(m_text_width, h);
        m_size.rwidth() += 10 + 32 + 10 + 10;
    }
    else {
        m_size.setHeight(h);
    }
    
    m_size.rheight() += 10 + 10;
}

void Button::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen(QColor(200, 200, 220, 255));
    pen.setWidth(2);

    p->setPen(pen);
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(QBrush(QColor(static_cast<int>(m_brightness),
                             static_cast<int>(m_brightness),
                             static_cast<int>(m_brightness), 100)));

    p->drawRoundedRect(1, 1,
                     m_size.width() - 2,
                     m_size.height() - 2,
                     2000 / m_size.width(),
                     2000 / m_size.height(), Qt::RelativeSize);

    p->drawPixmap(10,                        // x coordinate of the icon.
                  m_size.height() / 2 - 16,  // y coordinate of the icon.
                  32,                        // Icon's width.
                  32,                        // Icon's height.
                  m_icon.pixmap(32, 32));    // Icon's pixmap source.

    p->setFont(m_font);

    // 42 is the distance from the button's left margin to the
    // icon's right margin. The icon's x coordinate is 10 and its
    // width is 32.
    qreal x = boundingRect().x() + 42;
    qreal y = boundingRect().y();
    qreal w = boundingRect().width() - 42;
    qreal h = boundingRect().height();

    // This is the rectangle which ranges from the icon's right margin
    // to the button's right margin.
    QRectF rect(x, y, w, h);

    p->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
    // updateEditor();
}

QRectF Button::boundingRect() const
{
    return QRectF(1, 1, m_size.width() - 2, m_size.height() - 2);
}

QSize Button::size() const
{
    return m_size;
}

void Button::onMousePress()
{
    if (!m_down) {
        m_down = true;
        if (m_animation) {
            m_animation->abort();
        }
        m_brightness = BRIGHTNESS_DOWN;
        
        update();
    }
}

void Button::onMouseRelease()
{
    if (m_down) {
        m_down = false;
        if (m_animation) {
            m_animation->abort();
        }
        m_brightness = BRIGHTNESS_NORMAL;
        update();
    }
}

void Button::onMouseMove()
{
    if (!m_hover) {
        m_hover = true;
        
        if (m_down) {
            if (m_animation) {
                m_animation->abort();
            }
            m_brightness = BRIGHTNESS_HOVER;
        }
        else if (m_animation) {
            m_animation->setBrightness(BRIGHTNESS_HOVER);
        }
        else {
            m_animation = new ButtonAnimation(this, BRIGHTNESS_HOVER);
            Animator::instance()->add(m_animation);
        }
        
        update();
    }
}

void Button::onMouseLeave()
{
    if (m_hover) {
        m_hover = false;
        
        if (m_down) {
            if (m_animation) {
                m_animation->abort();
            }
            m_brightness = BRIGHTNESS_NORMAL;
        }
        else if (m_animation) {
            m_animation->setBrightness(BRIGHTNESS_NORMAL);
        }
        else {
            m_animation = new ButtonAnimation(this, BRIGHTNESS_NORMAL);
            Animator::instance()->add(m_animation);
        }
        
        update();
    }
}

bool Button::onClicked()
{
    if (true) {
        qCDebug(KNAVALBATTLE_LOG) << "clicked";
        Q_EMIT clicked();
        return true;
    }
    else {
        return false;
    }
}

void Button::setText(const QString& text)
{
    m_text = text;
    update();
}

double Button::brightness() const
{
    return m_brightness;
}

void Button::setBrightness(double value)
{
    m_brightness = value;
    update();
}

// ------------

double ButtonAnimation::m_speed = 0.46;

ButtonAnimation::ButtonAnimation(Button* button, int brightness)
: m_button(button)
, m_brightness(brightness)
{
    m_last = -1;
}

void ButtonAnimation::start(int t)
{
    m_last = t;
}

bool ButtonAnimation::step(int t)
{
    if (m_last == -1) {
        return true;
    }
    
    int sign = (m_button->brightness() > m_brightness) ? -1 : 1;
    double delta = (t - m_last) * m_speed;
//     qCDebug(KNAVALBATTLE_LOG) << "button step t =" << t << "sign =" << sign <<  "delta =" << delta;
    m_last = t;
    if (fabs(m_button->brightness() - m_brightness) <= delta) {
        m_button->setBrightness(m_brightness);
        return true;
    }
    else {
        m_button->setBrightness(m_button->brightness() + sign * delta);
        return false;
    }
}

void ButtonAnimation::abort()
{
    m_last = -1;
}

void ButtonAnimation::setBrightness(int value)
{
    m_brightness = value;
}

ButtonAnimation::~ButtonAnimation()
{
}




