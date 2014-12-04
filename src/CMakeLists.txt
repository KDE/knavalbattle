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
    welcomescreen.cpp)


kconfig_add_kcfg_files(knavalbattle_SRCS settingsbase.kcfgc)
kde4_add_app_icon(knavalbattle_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/pictures/hi*-app-knavalbattle.png")
add_executable(knavalbattle ${knavalbattle_SRCS} ${kwelcomescreen_SRCS})

target_link_libraries(knavalbattle Qt5::Xml Qt5::Qml Qt5::Network KF5::KDELibs4Support KF5::TextWidgets KF5::DNSSD Phonon::phonon4qt5 KF5KDEGames)

install(TARGETS knavalbattle  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES knavalbattleui.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/knavalbattle )
install(PROGRAMS org.kde.knavalbattle.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES knavalbattle.protocol  DESTINATION ${SERVICES_INSTALL_DIR})
