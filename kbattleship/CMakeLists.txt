set(kbattleship_SRCS
    controller.cpp
    coord.cpp
    element.cpp
    ship.cpp
    battlefield.cpp
    sea.cpp
    kbsrenderer.cpp
    sprite.cpp
    spritefactory.cpp
    battlefieldview.cpp
    seaview.cpp
    mainwindow.cpp
    main.cpp)

kde4_automoc(${kbattleship_SRCS})

kde4_add_executable(kbattleship ${kbattleship_SRCS})

target_link_libraries(kbattleship ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kbattleship  DESTINATION ${BIN_INSTALL_DIR} )



