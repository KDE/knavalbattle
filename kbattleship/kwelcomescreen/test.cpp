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
#include "kwelcomescreen.h"
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <KApplication>
#include <KAboutData>
#include <KLocale>
#include <KCmdLineArgs>
#include <KIcon>

static KCmdLineOptions options[] =
    {
        { "s", 0, 0 },
        { "set-arbitrary-names", I18N_NOOP( "Fill in the fildes to speed up testing. Just for development purpose." ), 0 },
        KCmdLineLastOption
    };


int main(int argc, char *argv[])
{
    KAboutData aboutData ( "kwelcomescreen-test", I18N_NOOP( "KWelcomeScreenTest" ),
                           "0.1", I18N_NOOP("Test class for KWelcomeScreen"), KAboutData::License_GPL,
                           "(c) 2007, Riccardo Iaconelli", 0, "http://games.kde.org/", "ruphy@fsfe.org" );
    aboutData.addAuthor( "Riccardo Iaconelli", 0, "ruphy@fsfe.org" );

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    KWelcomeScreen *test = new KWelcomeScreen();
    QGridLayout *testLayout = new QGridLayout(test);
    QLabel *icon1 = new QLabel(test);
    icon1->setPixmap(KIcon("about-kde").pixmap(128, 128));
    QLabel *icon2 = new QLabel(test);
    icon2->setPixmap(KIcon("kwrite").pixmap(128, 128));
    QLabel *icon3 = new QLabel(test);
    icon3->setPixmap(KIcon("okular").pixmap(128, 128));
    QLabel *icon4 = new QLabel(test);
    icon4->setPixmap(KIcon("k3b").pixmap(128, 128));
    QLabel *icon5 = new QLabel(test);
    icon5->setPixmap(KIcon("go-home").pixmap(128, 128));
    QLabel *icon6 = new QLabel(test);
    icon6->setPixmap(KIcon("laptop").pixmap(128, 128));
    QLabel *icon7 = new QLabel(test);
    icon7->setPixmap(KIcon("video-television").pixmap(128, 128));
    QLabel *icon8 = new QLabel(test);
    icon8->setPixmap(KIcon("audio-input-microphone").pixmap(128, 128));
    QLabel *icon9 = new QLabel(test);
    icon9->setPixmap(KIcon("cdwriter-unmount").pixmap(128, 128));
    testLayout->addWidget(icon1, 0, 0);
    testLayout->addWidget(icon2, 0, 1);
    testLayout->addWidget(icon3, 0, 2);
    testLayout->addWidget(icon4, 1, 0);
    testLayout->addWidget(icon5, 1, 1);
    testLayout->addWidget(icon6, 1, 2);
    testLayout->addWidget(icon7, 2, 0);
    testLayout->addWidget(icon8, 2, 1);
    testLayout->addWidget(icon9, 2, 2);
    test->addButton("Test Button 1", KIcon("plasmagik"), "tstbtn1", 0, 0);
    test->addButton("Test Button 2", KIcon("plasmagik"), "tstbtn2", 1, 0);
    test->addButton("Test Button 3", KIcon("plasmagik"), "tstbtn3", 0, 1);
    test->addButton("Test Button 4", KIcon("plasmagik"), "tstbtn4", 1, 1);
    test->init();
    test->resize(128*3, 128*3);
    test->show();

    return app.exec();
}
