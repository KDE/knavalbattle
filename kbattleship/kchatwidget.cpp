/***************************************************************************
                              kchatwidget.cpp
                             -----------------
    Developers: (c) 2000 Nikolas Zimmermann <wildfox@kde.org>
                (c) 2000 Daniel Molkentin <molkentin@kde.org>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kchatwidget.h"

#include "kchatwidget.moc"

#include <qmultilineedit.h>

KChatWidget::KChatWidget( QWidget *parent, const char *name ) : chatWidget( parent, name )
{
   connect( sendBtn, SIGNAL( clicked() ), SLOT( slotComputeMessage() ) );
    connect( commentEdit, SIGNAL( returnPressed() ), SLOT( slotComputeMessage() ) );

}

KChatWidget::~KChatWidget()
{
}

void KChatWidget::slotComputeMessage()
{
    chatView->setText(commentEdit->text());
    commentEdit->setText("");
}

