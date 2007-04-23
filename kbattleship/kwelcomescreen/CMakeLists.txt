
 set(kwelcomescreen_SRCS
     kwelcomescreen.cpp
     )

kde4_automoc(${kwelcomescreen_SRCS})
kde4_add_library(kwelcomescreen SHARED ${kwelcomescreen_SRCS})

target_link_libraries(kwelcomescreen ${KDE4_KDEUI_LIBS})
set_target_properties(kwelcomescreen PROPERTIES VERSION 0.0.0 SOVERSION 0 )


install(FILES kwelcomescreen.h DESTINATION ${INCLUDE_INSTALL_DIR})
install(TARGETS kwelcomescreen DESTINATION ${LIB_INSTALL_DIR})
