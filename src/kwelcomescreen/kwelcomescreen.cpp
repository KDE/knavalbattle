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

#include <kwelcomescreen.h>

#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QGridLayout>
#include <QVector>
#include <QIcon>
// #include <QFontMetrics>
#include <KDebug>

KWelcomeScreenOverlay::KWelcomeScreenOverlay(const QFont& font)
: m_background_color(0, 0, 0, 80)
, m_font(font)
, m_hover_button(0)
{

}

KWelcomeScreenOverlay::~KWelcomeScreenOverlay()
{
    foreach (KWelcomeScreenOverlayButton* button, m_buttons) {
        delete button;
    }
}

void KWelcomeScreenOverlay::resize(const QSize& sz) 
{
    m_size = sz;
    foreach (KWelcomeScreenOverlayButton* button, m_buttons) {
        button->onMouseLeave();
    }
}

void KWelcomeScreenOverlay::drawScreen(QPainter& p)
{
    QBrush b(m_background_color);
    p.setBrush(b);
    p.setPen(Qt::NoPen);
    p.drawRect(QRect(QPoint(0, 0), m_size));
    
    // draw buttons
    int n = m_buttons.size();
    if (n > 0) {
        int step = m_size.height() / n;
        int h = 0;
        for (int i = 0; i < n; i++) {
            KWelcomeScreenOverlayButton *button = m_buttons[i];
            QPoint pos((m_size.width() - button->rect().width()) / 2, 
                        h + (step - button->rect().height()) / 2);
            p.translate(pos);
            button->drawButton(p);
            p.translate(-pos);
            
            h += step;
        }
    }
}

void KWelcomeScreenOverlay::invalidateButton(KWelcomeScreenOverlayButton *b)
{
    int n = m_buttons.size();
    if (n > 0) {
        int step = m_size.height() / n;
        int h = 0;
        for (int i = 0; i < n; i++) {
            KWelcomeScreenOverlayButton *button = m_buttons[i];
            if (button == b) {
                QPoint pos((m_size.width() - button->rect().width()) / 2, 
                            h + (step - button->rect().height()) / 2);
                invalidateScreen(b->rect().translated(pos));
            }
            
            h += step;
        }
    }
}

void KWelcomeScreenOverlay::onMousePress(const QPoint &p)
{
    QPoint pos = p;
    KWelcomeScreenOverlayButton *button = at(pos);
    if (button) {
        button->onMousePress(pos);
    }
}

void KWelcomeScreenOverlay::onMouseRelease(const QPoint &p)
{
    QPoint pos = p;
    KWelcomeScreenOverlayButton *button = at(pos);
    if (button) {
        button->onMouseRelease(pos);
    }
}

void KWelcomeScreenOverlay::onMouseMove(const QPoint &p)
{
    QPoint pos = p;
    KWelcomeScreenOverlayButton *button = at(pos);
    
    if (m_hover_button && button != m_hover_button) {
        m_hover_button->onMouseLeave();
    }
    m_hover_button = button;
    
    if (button) {
        button->onMouseMove(pos);
    }
}

void KWelcomeScreenOverlay::onMouseLeave()
{
    if (m_hover_button) {
        m_hover_button->onMouseLeave();
    }
    m_hover_button = 0;
}

KWelcomeScreenOverlayButton *KWelcomeScreenOverlay::at(QPoint &p)
{
    int n = m_buttons.size();
    if (n == 0) {
        return 0;
    }
    
    int step = m_size.height() / n;
    int h = 0;
    for (int i = 0; i < n; i++) {
        KWelcomeScreenOverlayButton *button = m_buttons[i];
        QPoint pos((m_size.width() - button->rect().width()) / 2, 
                    h + (step - button->rect().height()) / 2);
        if (button->rect().translated(pos).contains(p)) {
            p = pos - button->rect().topLeft();
            return button;
        }
        h += step;
    }
    
    return 0;
}

KWelcomeScreenOverlayButton* KWelcomeScreenOverlay::createButton(const QString &text, const QIcon &icon)
{
    return new KWelcomeScreenOverlayButton(this, m_font, text, icon);
}

KWelcomeScreenOverlayButton *KWelcomeScreenOverlay::addButton(const QString &text, const QIcon &icon)
{
    KWelcomeScreenOverlayButton *button = createButton(text, icon);
    m_buttons.push_back(button);
    return button;
}

KWelcomeScreenOverlayButton::KWelcomeScreenOverlayButton(
    KWelcomeScreenOverlay *parent, const QFont &font, const QString &text, const QIcon &icon)
: m_parent(parent)
, m_text(text)
, m_icon(icon)
, m_enabled(true)
, m_opacity(1.0)
{
    QFontMetrics fm(font);
    int h = fm.height();
    if (h < 32) {
        h = 32;
    }
    m_rect = QRect(0, 0, fm.width(m_text), h);
    m_rect.adjust(0, 0, 10+32+10+10, 10+10);
}

void KWelcomeScreenOverlayButton::setEnabled(bool value)
{
    m_enabled = value;
    m_parent->invalidateButton(this);
}

void KWelcomeScreenOverlayButton::setOpacity(double value)
{
    m_opacity = value;
    m_parent->invalidateButton(this);
}

void KWelcomeScreenOverlayButton::drawButton(QPainter& p)
{
    p.setOpacity(m_opacity);
    
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor(200, 200, 220, 255));
    pen.setWidth(2);
    p.setPen(pen);
    switch (m_enabled ? m_state : UP) {
    case DOWN:
        p.setBrush(QBrush(QColor(200, 200, 200, 100)));
        break;
    case HOVER:
        p.setBrush(QBrush(QColor(100, 100, 100, 100)));
        break;
    case UP:
        p.setBrush(QBrush(QColor(0, 0, 0, 100)));
        break;
    }
    p.drawRoundRect(QRectF(0.0, 0.0, m_rect.width() - 2, m_rect.height() -2), 8, 40);
    p.drawPixmap(10, (m_rect.height()/2-16), 32, 32, m_icon.pixmap(32, 32));

    //FIXME calculate the position basing on the text height.
    p.drawText(10+32+10, (m_rect.height()/2+6), m_text);
    
    p.setOpacity(1.0);
}

void KWelcomeScreenOverlayButton::onMousePress(const QPoint &)
{
    m_state = DOWN;
    m_parent->invalidateButton(this);
}

void KWelcomeScreenOverlayButton::onMouseRelease(const QPoint &)
{
    if (m_state == DOWN) {
        m_parent->buttonClicked(this);
    }
    m_state = HOVER;
    m_parent->invalidateButton(this);
}

void KWelcomeScreenOverlayButton::onMouseMove(const QPoint &)
{
    m_state = m_state == DOWN ? DOWN : HOVER;
    m_parent->invalidateButton(this);
}

void KWelcomeScreenOverlayButton::onMouseLeave()
{
    m_state = UP;
    m_parent->invalidateButton(this);
}

KWelcomeScreen::KWelcomeScreen(const QString& applicationName, QWidget *parent)
    : QWidget(parent)
{
//     QWidget *blackWidget = new QWidget;
//     QPalette palette = blackWidget->palette();
//     palette.setColor(QPalette::Window, QColor(0, 0, 0, 180));
//     blackWidget->setPalette(palette);
//     blackWidget->raise();
    m_applicationName = applicationName;
    widget = new KWelcomeWidget(applicationName, this);

    mainLayout = new QGridLayout;
    mainLayout->addWidget(widget);
    setLayout(mainLayout);
    connect(widget, SIGNAL(clicked(int)), this, SIGNAL(buttonClicked(int)));
//     setCentralWidget(widget);
}

void KWelcomeScreen::init()
{
    widget->raise();
}


void KWelcomeScreen::hideOverlay()
{
    widget->hide();
}

void KWelcomeScreen::showOverlay()
{
    widget->show();
}

void KWelcomeScreen::addButton(const QString &text, const QIcon &icon)
{
    widget->addButton(text, icon);
}

KWelcomeWidget::KWelcomeWidget(const QString& applicationName, QWidget *parent)
    : QWidget(parent)
    , KWelcomeScreenOverlay(font())
{
    m_applicationName = applicationName;
    buttons.reserve(10);
    buttons.fill(NULL);
    
    setMouseTracking(true);
    
//     KWelcomeScreenPrivate *d = new KWelcomeScreenPrivate;
//     layout = new QGridLayout;
//     layout->setSizeConstraint(QLayout::SetMinimumSize);
//     layout->setSizeConstraint(QLayout::SetFixedSize);
//     frame = new QWidget;
//     frame->setLayout(layout);
//     QGridLayout *mainLayout = new QGridLayout;
//     mainLayout->addWidget(frame);
//     setLayout(mainLayout);
//     resize(418, 418);
}

void KWelcomeWidget::resizeEvent(QResizeEvent *event)
{
    KWelcomeScreenOverlay::resize(event->size());
}

void KWelcomeWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    KWelcomeScreenOverlay::drawScreen(p);
#if 0 // TODO: restore this stuff
    QFont font;
    font.setWeight(QFont::Bold);
    font.setStyle(QFont::StyleItalic);
    font.setPointSize(25);
    QFontMetrics m(font);
    QPen pen(QColor(200, 200, 220, 255));
    pen.setWidth(20);
    p.setPen(pen);
    p.setFont(font);
    p.drawText(width()/2-(m.width(m_applicationName)/2), m.height()+20, m_applicationName);
#endif
}

void KWelcomeWidget::mouseMoveEvent(QMouseEvent *event)
{
    onMouseMove(event->pos());
}

void KWelcomeWidget::leaveEvent(QEvent *)
{
    onMouseLeave();
}

void KWelcomeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        onMousePress(event->pos());
    }
}

void KWelcomeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        onMouseRelease(event->pos());
    }
}

void KWelcomeWidget::invalidateScreen(const QRect& rect)
{
    update(rect);
}

void KWelcomeWidget::buttonClicked(KWelcomeScreenOverlayButton* button)
{
    for (int i = 0; i < m_buttons.size(); i++) {
        if (button == m_buttons[i])
            emit clicked(i);
    }
}

#include "kwelcomescreen.moc"

