include_directories(${CMAKE_SOURCE_DIR}/libkdegames ${CMAKE_SOURCE_DIR}/libkdegames/highscore ${CMAKE_CURRENT_SOURCE_DIR}/kwelcomescreen)

 set(kwelcomescreen_SRCS
     kwelcomescreen/kwelcomescreen.cpp
     )

kde4_automoc(${kwelcomescreen_SRCS})

# currently not needed by anything else, so in benefit of the doubt, don't install it
#kde4_add_library(kwelcomescreen SHARED ${kwelcomescreen_SRCS})
#target_link_libraries(kwelcomescreen ${KDE4_KDEUI_LIBS})
#install(FILES kwelcomescreen.h DESTINATION ${INCLUDE_INSTALL_DIR})
#install(TARGETS kwelcomescreen DESTINATION ${LIB_INSTALL_DIR})

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
    coord.cpp
    element.cpp
    entity.cpp
    entitylabel.cpp
    gamechooser.cpp
    generalcontroller.cpp
    kbsrenderer.cpp
    main.cpp
    mainwindow.cpp
    message.cpp
    networkentity.cpp
    playerentity.cpp
    playfield.cpp
    protocol.cpp
    sea.cpp
    seaview.cpp
    ship.cpp
    shot.cpp
    sprite.cpp
    spritefactory.cpp
    stats.cpp
    uientity.cpp
    welcomescreen.cpp)


kde4_automoc(${kbattleship_SRCS})

kde4_add_executable(kbattleship ${kbattleship_SRCS} ${kwelcomescreen_SRCS})

target_link_libraries(kbattleship ${KDE4_KDEUI_LIBS} ${KDE4_PHONON_LIBS} kdegames)

install(TARGETS kbattleship  DESTINATION ${BIN_INSTALL_DIR} )
install( FILES kbattleshipui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbattleship )
install(FILES kbattleship.desktop DESTINATION ${XDG_APPS_DIR})



