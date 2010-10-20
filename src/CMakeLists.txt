include_directories(${CMAKE_SOURCE_DIR}/libkdegames/highscore ${CMAKE_CURRENT_SOURCE_DIR}/kwelcomescreen)

set(kbattleship_SRCS
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
    shot.cpp
    simplemenu.cpp
    sprite.cpp
    spritefactory.cpp
    stats.cpp
    statswidget.cpp
    uientity.cpp
    welcomescreen.cpp)


kde4_add_kcfg_files(kbattleship_SRCS settingsbase.kcfgc)

kde4_add_app_icon(kbattleship_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/pictures/hi*-app-kbattleship.png")
kde4_add_executable(kbattleship ${kbattleship_SRCS} ${kwelcomescreen_SRCS})

target_link_libraries(kbattleship ${QT_QTXML_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${KDE4_KDEUI_LIBS} ${KDE4_PHONON_LIBS} ${KDE4_KDNSSD_LIBS} kdegames ${KGGZGAMES_LIBS} )

install(TARGETS kbattleship  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES kbattleshipui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbattleship )
install(PROGRAMS kbattleship.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES kbattleship.protocol  DESTINATION ${SERVICES_INSTALL_DIR})

register_ggz_module(module.dsc)



