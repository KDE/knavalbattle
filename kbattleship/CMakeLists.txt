
add_subdirectory( dialogs ) 
add_subdirectory( pictures ) 
add_subdirectory( sounds ) 

include_directories(dialogs/ ${CMAKE_SOURCE_DIR}/libkdegames ${CMAKE_SOURCE_DIR}/libkdegames/highscore  ) 


########### next target ###############

set(kbattleship_SRCS ${kbattleship_SRCS}
   kbaiplayer.cpp 
   kbstrategy.cpp 
   kbverticalstepstrategy.cpp 
   kbdestroyshipstrategy.cpp 
   kbhorizontalstepstrategy.cpp 
   kbrandomshotstrategy.cpp 
   kbdiagonalwrapstrategy.cpp 
   kmessage.cpp 
   kbattleshipserver.cpp 
   kbattleshipclient.cpp 
   kbattleshipview.cpp 
   kgridwidget.cpp 
   kbattlefield.cpp 
   kchatwidget.cpp 
   kserverdialog.cpp 
   kclientdialog.cpp 
   kstatdialog.cpp 
   kbdiagonalshotstrategy.cpp 
   konnectionhandling.cpp 
   kship.cpp 
   kshiplist.cpp 
   kbchooserstrategy.cpp 
   kbattleship.cpp 
   main.cpp )

kde4_add_ui_files(kbattleship_SRCS 
   dialogs/statDlg.ui 
   dialogs/connectDlg.ui 
   dialogs/serverDlg.ui 
   dialogs/chatDlg.ui 
   dialogs/infoDlg.ui)

kde4_automoc(${kbattleship_SRCS})

kde4_add_executable(kbattleship ${kbattleship_SRCS})

target_link_libraries(kbattleship  ${KDE4_KDEUI_LIBS} kdegames ${KDE4_KDNSSD_LIBS} knotifyconfig )

install(TARGETS kbattleship  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kbattleship.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kbattleshipui.rc kbattleship.notifyrc  DESTINATION  ${DATA_INSTALL_DIR}/kbattleship )
install( FILES _kbattleship._tcp  DESTINATION  ${DATA_INSTALL_DIR}/zeroconf )


