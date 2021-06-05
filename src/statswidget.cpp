/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include "statswidget.h"
#include "stats.h"
#include "kbsrenderer.h"

#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QHBoxLayout>


StatsWidgetElement::StatsWidgetElement(const QPixmap &icon, const QString &text, QWidget *parent)
: QFrame(parent)
{
    setData(icon, text);
}

void StatsWidgetElement::paintEvent(QPaintEvent * /*ev*/)
{
    QPen pen;
    pen.setColor(QColor(0, 0, 0, 255));

    QRect rect;
    rect.setTopLeft(QPoint(BORDER_X, BORDER_Y));
    rect.setWidth(frameRect().width()   - BORDER_WIDTH);
    rect.setHeight(frameRect().height() - BORDER_WIDTH);

    QPainter p(this);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(rect, 1000 / rect.width(), 1000 / rect.height(), Qt::RelativeSize);
}

void StatsWidgetElement::setText(const QString &text)
{
    QString value = QLatin1String("<font size=\"5\">") + text + QLatin1String("</font>");
    m_text->setText(value);
}

void StatsWidgetElement::setData(const QPixmap &icon, const QString &text)
{
    m_icon = new QLabel;
    m_text = new QLabel;

    m_icon->setPixmap(icon);
    setText(text);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addSpacing(MARGIN);
    layout->addWidget(m_icon);
    layout->addStretch(width() - m_icon->width() - m_text->width());
    layout->addWidget(m_text);
    layout->addSpacing(MARGIN);
    setLayout(layout);
}

StatsWidget::StatsWidget(KBSRenderer *renderer, QWidget *parent)
: QWidget(parent)
, m_stats(nullptr)
{
    m_elements[0] = new StatsWidgetElement(
        renderer->spritePixmap(QStringLiteral("water-impact"), QSize(32, 32)),
        QStringLiteral("0"),
        this);
    m_elements[0]->show();
    m_elements[1] = new StatsWidgetElement(
        renderer->spritePixmap(QStringLiteral("hit"), QSize(32, 32)),
        QStringLiteral("0"),
        this);
    m_elements[1]->show();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_elements[0]);
    layout->addWidget(m_elements[1]);
    setLayout(layout);
}

void StatsWidget::setWidth(int width)
{
    int x = geometry().x();
    int y = geometry().y();
    int h = geometry().height();

    setGeometry(x, y, width, h);
}

void StatsWidget::refresh()
{
    if (m_stats) {
        m_elements[0]->setText(QString::number(m_stats->misses()));
        m_elements[1]->setText(QString::number(m_stats->hits()));
    }
    else {
        m_elements[0]->setText(QStringLiteral("0"));
        m_elements[1]->setText(QStringLiteral("0"));
    }
}

void StatsWidget::setData(Stats* stats)
{
    m_stats = stats;
    if (m_stats) {
        connect(m_stats.data(), &Stats::hitsChanged, this, &StatsWidget::refresh);
        connect(m_stats.data(), &Stats::missesChanged, this, &StatsWidget::refresh);
    }
   refresh();
}




