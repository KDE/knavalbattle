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
    CATEGORY_NAME knavalbattle
    OLD_CATEGORY_NAMES org.kde.kdegames.knavalbattle
    DESCRIPTION "KNavalBattle game"
    EXPORT KNAVALBATTLE
)

kconfig_add_kcfg_files(knavalbattle settingsbase.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/pictures/*-apps-knavalbattle.png")
ecm_add_app_icon(knavalbattle ICONS ${ICONS_SRCS})

target_link_libraries(knavalbattle
    KDEGames6
    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
    KF6::Completion
    KF6::TextWidgets
    KF6::DNSSD
    KF6::Crash
    KF6::DBusAddons
    Qt6::Xml
    Qt6::Network
)

install(TARGETS knavalbattle  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.knavalbattle.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.knavalbattle.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
