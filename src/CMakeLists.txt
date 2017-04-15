include_directories(${CMAKE_SOURCE_DIR}/KF5KDEGames/highscore ${CMAKE_CURRENT_SOURCE_DIR}/kwelcomescreen)

set(knavalbattle_SRCS
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
)


kconfig_add_kcfg_files(knavalbattle_SRCS settingsbase.kcfgc)
file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/pictures/*-apps-knavalbattle.png")
ecm_add_app_icon(knavalbattle_SRCS ICONS ${ICONS_SRCS})
add_executable(knavalbattle ${knavalbattle_SRCS} ${kwelcomescreen_SRCS})

target_link_libraries(knavalbattle
    Qt5::Xml
    Qt5::Network
    KF5::I18n
    KF5::XmlGui
    KF5::Completion
    KF5::TextWidgets
    KF5::DNSSD
    KF5::Crash
    KF5::DBusAddons
    KF5KDEGames
)

install(TARGETS knavalbattle  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES knavalbattleui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/knavalbattle)
install(PROGRAMS org.kde.knavalbattle.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES knavalbattle.protocol  DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES org.kde.knavalbattle.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
