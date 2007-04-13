find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories(${LIBRARY_OUTPUT_PATH} ${KDE4_LIB_DIR})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

 set(kwelcomescreen_SRCS
     test.cpp
     kwelcomescreen.cpp
     )

kde4_automoc(${kwelcomescreen_SRCS})
kde4_add_executable(kwelcomescreen ${kwelcomescreen_SRCS})

target_link_libraries(kwelcomescreen ${QT_GUI} ${KDE4_KDEUI_LIBS})

install(TARGETS kwelcomescreen DESTINATION ${BIN_INSTALL_DIR})
