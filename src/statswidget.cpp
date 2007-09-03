/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "statswidget.h"
#include "kbsrenderer.h"
#include <QColor>

RoundFrame::RoundFrame(const QSize& size, KGameCanvasAbstract* parent)
: KGameCanvasItem(parent)
, m_size(size)
{
}

void RoundFrame::paint(QPainter* p) 
{
    p->setRenderHint(QPainter::Antialiasing);
    p->drawRoundRect(rect(), 1000 / rect().width(), 1000 / rect().height());
}

QRect RoundFrame::rect() const 
{
    return QRect(pos(), m_size);
}

void RoundFrame::setSize(const QSize& size)
{
    m_size = size;
    if (visible() && canvas())
        changed();
}

StatsWidgetElement::StatsWidgetElement(const QPixmap& pixmap, const QString& text, 
    KGameCanvasAbstract* parent)
: KGameCanvasGroup(parent)
, m_width(0)
, m_metrics(QFont())
{
    m_icon = new KGameCanvasPixmap(pixmap, this);
    m_icon->show();
    
    QFont font;
    font.setPointSize(15);
    m_metrics = QFontMetrics(font);
    m_text = new KGameCanvasText(text, Qt::black, font,
        KGameCanvasText::HRight, KGameCanvasText::VCenter,  this);
    m_text->show();   

    m_frame = new RoundFrame(QSize(0, 0), this);
    m_frame->show();
}

void StatsWidgetElement::setText(const QString& text)
{
    m_text->setText(text);
    update();
}

int StatsWidgetElement::minimumWidth() const
{
    return HEIGHT + MARGIN * 3 + m_metrics.width(m_text->text());
}

void StatsWidgetElement::update()
{
    int width = m_width;
    int min = minimumWidth();
    if (m_width < min) {
        width = min;
    }
        
    m_size = QSize(width, HEIGHT);
    
    m_frame->setSize(m_size);
    m_frame->moveTo(0, 0);
    m_icon->moveTo(MARGIN, MARGIN / 2);
    m_text->moveTo(width - MARGIN, HEIGHT / 2);
}

void StatsWidgetElement::setWidth(int width)
{
    if (width != m_size.width()) {
        m_width = width;
        update();
    }
}

QSize StatsWidgetElement::size() const
{
    return m_size;
}

StatsWidget::StatsWidget(int width, KBSRenderer* renderer, KGameCanvasAbstract* parent)
: KGameCanvasGroup(parent)
, m_width(width)
{
    m_elements[0] = new StatsWidgetElement(
        renderer->render("water-impact", QSize(32, 32)),
        "0",
        this);
    m_elements[0]->show();
    m_elements[1] = new StatsWidgetElement(
        renderer->render("hit", QSize(32, 32)),
        "0",
        this);
    m_elements[1]->show();
        
    update();
}

void StatsWidget::setWidth(int width)
{
    if (width != m_width) {
        m_width = width;
        update();
    }
}

void StatsWidget::update()
{
    int element_width = (m_width - MARGIN*3) / 2;
    m_elements[0]->moveTo(MARGIN, 0);
    m_elements[0]->setWidth(element_width);
    
    m_elements[1]->moveTo(m_elements[0]->size().width() + MARGIN * 2, 0);
    m_elements[1]->setWidth(element_width);
}

void StatsWidget::setData(int miss, int hits) {
    m_elements[0]->setText(QString::number(miss));
    m_elements[1]->setText(QString::number(hits));
}


