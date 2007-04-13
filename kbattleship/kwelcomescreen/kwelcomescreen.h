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
#ifndef KWELCOMESCREEN_H
#define KWELCOMESCREEN_H

#include <QAbstractButton>

class QPaintEvent;
class QGridLayout;
class KWelcomeWidget;
class KWelcomeScreenButton;
class QEvent;

class KWelcomeScreen : public QWidget
{
    Q_OBJECT
public:
    KWelcomeScreen(QWidget *parent = 0);
    void init();
    void addButton(const QString &text, const QIcon &icon, const QString &shortText, int rowNum, int colNum);
private:
    KWelcomeWidget *widget;
//     KWelcomeScreenPrivate *d;
private Q_SLOTS:
    void buttonClickedDebug(QString shortText);
signals:
    void buttonClicked(QString shortText);
};

class KWelcomeWidget : public QWidget
{
    Q_OBJECT
public:
    KWelcomeWidget(QWidget *parent = 0);
    void addButton(const QString &text, const QIcon &icon, const QString &shortText, int rowNum, int colNum);
protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QGridLayout *layout;
    QWidget *frame;
    QVector<KWelcomeScreenButton *> buttons;
//     KWelcomeScreenPrivate *d;
signals:
    void buttonClicked(QString shortText);
};

// class KWelcomeScreenPrivate
// {
// friend class KWelcomeScreen;
// public:
//     KWelcomeScreenPrivate();
// private:
//     QWidget *children;
//     QString string;
// };

class KWelcomeScreenButton : public QAbstractButton
{
    Q_OBJECT
    
    Q_PROPERTY(bool autoraise READ autoraise WRITE setAutoraise)
public:
    KWelcomeScreenButton(QWidget *parent = 0);
    void setProprieties(const QString &text, const QIcon &icon, const QString &shortText);
    
    bool autoraise() const;
    void setAutoraise(bool value);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);
    QSize sizeHint();
private:
    QString text, shortText;
    QIcon icon;
    bool m_raised;
    bool m_autoraise;
private Q_SLOTS:
    void buttonClickedSlot();
signals:
    void buttonClicked(QString shortText);
};

#endif
