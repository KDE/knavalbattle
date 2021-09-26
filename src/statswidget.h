/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QLabel>
#include <QFrame>
#include <QWidget>
#include <QPointer>

class Stats;
class KBSRenderer;

class StatsWidgetElement : public QFrame 
{
public:
    static const int MARGIN         = 5;
    static const int BORDER_WIDTH   = 2;
    static const int BORDER_X       = 1;
    static const int BORDER_Y       = 1;

private:
    QLabel *m_icon;
    QLabel *m_text;

public:
    StatsWidgetElement(const QPixmap &icon, const QString &text, QWidget *parent);

    void setText(const QString &text);
    void setData(const QPixmap &icon, const QString &text);

protected:
    void paintEvent(QPaintEvent *) override;
};

class StatsWidget : public QWidget
{
    Q_OBJECT
private:
    QPointer<Stats> m_stats;
    
    StatsWidgetElement* m_elements[2];
public:
    StatsWidget(KBSRenderer* renderer, QWidget* parent);
    
    void setWidth(int width);
    void setData(Stats*);
public Q_SLOTS:
    void refresh();
};


#endif // STATSWIDGET_H
