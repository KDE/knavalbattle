/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <KDBusService>
#include <Kdelibs4ConfigMigrator>

#include <QApplication>
#include <QUrl>
#include <QCommandLineParser>
#include <QCommandLineOption>


#include "mainwindow.h"
#include "coord.h"
#include "statswidget.h"
#include "knavalbattle_version.h"

int main(int argc, char** argv)
{
    // Fixes blurry icons with fractional scaling
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("knavalbattle");

    Kdelibs4ConfigMigrator migrate(QStringLiteral("knavalbattle"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("knavalbattlerc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("knavalbattleui.rc"));
    migrate.migrate();

    KAboutData aboutData(QStringLiteral("knavalbattle"), i18n("Naval Battle"),
        QStringLiteral(KNAVALBATTLE_VERSION_STRING),
        i18n("The KDE ship sinking game"), KAboutLicense::GPL, 
        i18n("(c) 2000-2005  Nikolas Zimmermann, Daniel Molkentin\n"
              "(c) 2007 Paolo Capriotti"),
        QString(),
        QStringLiteral("https://apps.kde.org/knavalbattle") );

    aboutData.addAuthor(i18n("Paolo Capriotti"), i18n("Current maintainer"), QStringLiteral("p.capriotti@gmail.com"));
    aboutData.addAuthor(i18n("Nikolas Zimmermann"), i18n("Project Founder, GUI Handling, Client/Server"), QStringLiteral("wildfox@kde.org"));
    aboutData.addAuthor(i18n("Daniel Molkentin"), i18n("Dialog Stuff, Client/Server"), QStringLiteral("molkentin@kde.org"));
    aboutData.addAuthor(i18n("Kevin Krammer"), i18n("Computer Player"), QStringLiteral("kevin.krammer@gmx.at"));
    
    aboutData.addCredit(i18n("Johann Ollivier Lapeyre"), i18n("Game artwork"), QStringLiteral("johann.ollivierlapeyre@gmail.com"));
    aboutData.addCredit(i18n("Eugene Trounev"), i18n("Background"), QStringLiteral("irs_me@hotmail.com"));
    aboutData.addCredit(i18n("Robert Wadley"), i18n("Artwork for some of the ships"), QStringLiteral("rob@robntina.fastmail.us"));
    aboutData.addCredit(i18n("Riccardo Iaconelli"), i18n("Welcome screen"), QStringLiteral("ruphy@fsfe.org"));
    aboutData.addCredit(i18n("Benjamin Adler"), i18n("Icon"), QStringLiteral("benadler@bigfoot.de"));
    aboutData.addCredit(i18n("Nils Trzebin"), i18n("Sounds"), QStringLiteral("nils.trzebin@stud.uni-hannover.de"));
    aboutData.addCredit(i18n("Elmar Hoefner"), i18n("GFX"), QStringLiteral("elmar.hoefner@uibk.ac.at"));
    aboutData.addCredit(i18n("Lukas Tinkl"), i18n("Non-Latin1 Support"), QStringLiteral("lukas@kde.org"));
    aboutData.addCredit(i18n("Malte Starostik"), i18n("Various improvements"), QStringLiteral("malte.starostik@t-online.de"));
    aboutData.addCredit(i18n("Albert Astals Cid"), i18n("Various improvements and bugfixes"), QStringLiteral("aacid@kde.org"));
    aboutData.addCredit(i18n("John Tapsell"), i18n("Various improvements and bugfixes"), QStringLiteral("john@geola.co.uk"));
    aboutData.addCredit(i18n("Inge Wallin"), i18n("Bugfixes and refactoring"), QStringLiteral("inge@lysator.liu.se"));
    aboutData.addCredit(i18n("Jakub Stachowski"), i18n("DNS-SD discovery"), QStringLiteral("qbast@go2.pl"));
    aboutData.addCredit(i18n("Roney Gomes"), i18n("Porting to KGameRenderer and QGraphicsView"), QStringLiteral("roney477@gmail.com"));
        
    aboutData.setOrganizationDomain(QByteArray("kde.org"));
    aboutData.setProductName(QByteArray("knavalbattle"));

    KAboutData::setApplicationData(aboutData);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("knavalbattle")));
  
    QCommandLineParser parser;
    
    parser.addPositionalArgument(i18n("[URL]"), i18n("URL of a Naval Battle game server to connect to after startup"));

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KCrash::initialize();
    KDBusService service; 
    
    qRegisterMetaType<Coord>("Coord");

    QUrl url;
    const QStringList &args = parser.positionalArguments();
    if (!args.isEmpty()) {
        for (int i = 0; i < args.count(); ++i) {
            url = QUrl(args.at(i)); 

            if (!url.isValid())
                continue;

            break;
        }
    }
    
    MainWindow* window = new MainWindow(url);
    //StatsWidget* window = new StatsWidget(0, 0);
    window->show();

    return app.exec();
}
