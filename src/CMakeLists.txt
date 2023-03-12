add_executable(knavalbattle)

target_sources(knavalbattle PRIVATE
    ai/ai.cpp
    ai/dummyai.cpp
    ai/smartai.cpp
    aientity.cpp
    animation.cpp
    animator.cpp
    audioplayer.cpp
    battlefield.cpp
    battlefieldview.cpp
    button.cpp
    chatwidget.cpp
    controller.cpp
    coord.cpp
    element.cpp
    entity.cpp
    kbsrenderer.cpp
    main.cpp
    mainwindow.cpp
    message.cpp
    networkdialog.cpp
    networkentity.cpp
    playerentity.cpp
    playerlabel.cpp
    playfield.cpp
    protocol.cpp
    sea.cpp
    seaview.cpp
    settings.cpp
    ship.cpp
    ships.cpp
    shot.cpp
    simplemenu.cpp
    sprite.cpp
    spritefactory.cpp
    stats.cpp
    statswidget.cpp
    uientity.cpp
    welcomescreen.cpp

    knavalbattle.qrc
)

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX KNAVALBATTLE VERSION_HEADER knavalbattle_version.h)

ecm_qt_declare_logging_category(knavalbattle
    HEADER knavalbattle_debug.h
    IDENTIFIER KNAVALBATTLE_LOG
    CATEGORY_NAME org.kde.kdegames.knavalbattle
    DESCRIPTION "knavalbattle (kdegames)"
    EXPORT KNAVALBATTLE
)

kconfig_add_kcfg_files(knavalbattle settingsbase.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/pictures/*-apps-knavalbattle.png")
ecm_add_app_icon(knavalbattle ICONS ${ICONS_SRCS})

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(knavalbattle KDEGames6)
else()
    target_link_libraries(knavalbattle KF5KDEGames)
endif()

target_link_libraries(knavalbattle
    Qt${QT_MAJOR_VERSION}::Xml
    Qt${QT_MAJOR_VERSION}::Network
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::Completion
    KF${KF_MAJOR_VERSION}::TextWidgets
    KF${KF_MAJOR_VERSION}::DNSSD
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::DBusAddons
)

install(TARGETS knavalbattle  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.knavalbattle.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.knavalbattle.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
