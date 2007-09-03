/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QFontMetrics>

#include <KGameCanvas>

class KBSRenderer;
class Stats;

class RoundFrame : public KGameCanvasItem
{
    QSize m_size;
public:
    RoundFrame(const QSize& size, KGameCanvasAbstract* canvas);
    virtual void paint(QPainter* p);
    virtual QRect rect() const;
    
    void setSize(const QSize& size);
};

class StatsWidgetElement : public KGameCanvasGroup
{
public:
    static const int HEIGHT = 40;
    static const int MARGIN = 8;
private:
    int m_width; // requested width
    QSize m_size; // actual width
    QFontMetrics m_metrics; // font metrics
    
    RoundFrame* m_frame;
    KGameCanvasPixmap* m_icon;
    KGameCanvasText* m_text;
    
    void update();
    int minimumWidth() const;
public:
    StatsWidgetElement(const QPixmap& icon, const QString& text, 
        KGameCanvasAbstract* parent);
        
    void setWidth(int width);
    void setText(const QString& text);
    QSize size() const;
};

class StatsWidget : public KGameCanvasGroup
{
public:
    static const int MARGIN = 10;
    static const int HEIGHT = StatsWidgetElement::HEIGHT;
private:
    int m_width;
    Stats* m_stats;
    
    StatsWidgetElement* m_elements[2];
public:
    StatsWidget(int width, KBSRenderer* renderer, KGameCanvasAbstract* parent);

    void setWidth(int width);
    void update();
    void setData(int miss, int hits);
};


#endif // STATSWIDGET_H

