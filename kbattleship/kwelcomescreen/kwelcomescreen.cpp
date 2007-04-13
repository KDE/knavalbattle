/**************************************************************************
*   Copyright (C) 2007 by Riccardo Iaconelli  <ruphy@fsfe.org>            *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; If not, write to                             *
*   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,  *
*   Boston, MA 02110-1301, USA.                                           *
***************************************************************************/

#include "kwelcomescreen.moc"
#include "kwelcomescreen.h"

#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QGridLayout>
#include <QVector>
#include <QIcon>
#include <KDebug>


KWelcomeScreen::KWelcomeScreen(QWidget *parent)
    : QWidget(parent)
{
//     QWidget *blackWidget = new QWidget;
//     QPalette palette = blackWidget->palette();
//     palette.setColor(QPalette::Window, QColor(0, 0, 0, 180));
//     blackWidget->setPalette(palette);
//     blackWidget->raise();
    widget = new KWelcomeWidget(this);
    connect(widget, SIGNAL(buttonClicked(QString)), this, SIGNAL(buttonClicked(QString)));
    connect(widget, SIGNAL(buttonClicked(QString)), this, SLOT(buttonClickedDebug(QString)));
}

void KWelcomeScreen::buttonClickedDebug(QString shortText)
{
    kDebug() << shortText << endl;
}

void KWelcomeScreen::init()
{
    widget->raise();
    // Connect all the buttons to the buttonClicked slot
}

void KWelcomeScreen::addButton(const QString &text, const QIcon &icon, const QString &shortText, int rowNum, int colNum)
{
    widget->addButton(text, icon, shortText, rowNum, colNum);
}

KWelcomeWidget::KWelcomeWidget(QWidget *parent)
    : QWidget(parent)
{
    buttons.reserve(10);
    buttons.fill(NULL);
//     KWelcomeScreenPrivate *d = new KWelcomeScreenPrivate;
    layout = new QGridLayout;
//     layout->setSizeConstraint(QLayout::SetMinimumSize);
//     layout->setSizeConstraint(QLayout::SetFixedSize);
    frame = new QWidget;
    frame->setLayout(layout);
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(frame);
    setLayout(mainLayout);
    resize(418, 418);
}

void KWelcomeWidget::addButton(const QString &text, const QIcon &icon, const QString &shortText, int rowNum, int colNum)
{
    buttons.resize((buttons.size()+1));
    buttons[buttons.size()-1] = new KWelcomeScreenButton(frame);
    buttons[buttons.size()-1]->setProprieties(text, icon, shortText);
    layout->addWidget(buttons.at(buttons.size()-1), rowNum, colNum);
    connect(buttons[buttons.size()-1], SIGNAL(buttonClicked(QString)), this, SIGNAL(buttonClicked(QString)));
}

void KWelcomeWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QBrush b(QColor(0, 0, 0, 180));
    p.setBrush(b);
    p.drawRect(event->rect());
    p.setBrush(Qt::black);
//     p.drawLine(0, 0, width(), height());
}

KWelcomeScreenButton::KWelcomeScreenButton(QWidget *parent)
    : QAbstractButton(parent)
{
    move(150, 50);
    connect(this, SIGNAL(clicked()), this, SLOT(buttonClickedSlot()));
}

void KWelcomeScreenButton::buttonClickedSlot()
{
    emit buttonClicked(shortText);
}

void KWelcomeScreenButton::setProprieties(const QString &text, const QIcon &icon, const QString &shortText)
{
    this->text = text;
    this->icon = icon;
    this->shortText = shortText;
}

QSize KWelcomeScreenButton::sizeHint()
{
    return QSize(150, 40);
}

void KWelcomeScreenButton::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor(200, 200, 220, 255));
    pen.setWidth(2);
    p.setPen(pen);
    if (this->isDown()) {
        p.setBrush(QBrush(QColor(200, 200, 200, 100)));
    } else {
        p.setBrush(QBrush(QColor(0, 0, 0, 100)));
    }
    p.drawRoundRect(QRectF(2.0, 2.0, sizeHint().rwidth(), sizeHint().rheight()), 8, 40);
    p.drawPixmap(10, (sizeHint().rheight()/2-14), 32, 32, icon.pixmap(32, 32));

    //FIXME calculate the position basing on the text height.
    p.drawText(10+32+10, (sizeHint().rheight()/2+6), text);
}

