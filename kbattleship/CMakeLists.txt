add_subdirectory( pictures ) 

set(kbattleship_SRCS
    ai/ai.cpp
    ai/dummyai.cpp
    ai/smartai.cpp
    animation.cpp
    animator.cpp
    battlefield.cpp
    battlefieldview.cpp
    controller.cpp
    coord.cpp
    element.cpp
    kbsrenderer.cpp
    main.cpp
    mainwindow.cpp
    message.cpp
    networkcontroller.cpp
    playfield.cpp
    protocol.cpp
    sea.cpp
    seaview.cpp
    ship.cpp
    sprite.cpp
    spritefactory.cpp
    stats.cpp)


kde4_automoc(${kbattleship_SRCS})

kde4_add_executable(kbattleship ${kbattleship_SRCS})

target_link_libraries(kbattleship ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kbattleship  DESTINATION ${BIN_INSTALL_DIR} )
install( FILES kbattleshipui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbattleship )



