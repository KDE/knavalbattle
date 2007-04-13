add_subdirectory(pictures)
add_subdirectory(sounds)

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
    coord.cpp
    element.cpp
    entity.cpp
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
    uientity.cpp)


kde4_automoc(${kbattleship_SRCS})

kde4_add_executable(kbattleship ${kbattleship_SRCS})

target_link_libraries(kbattleship ${KDE4_KDEUI_LIBS} ${KDE4_PHONONCORE_LIBS} kdegames)

install(TARGETS kbattleship  DESTINATION ${BIN_INSTALL_DIR} )
install( FILES kbattleshipui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbattleship )



