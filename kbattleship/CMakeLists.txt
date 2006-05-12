
add_subdirectory( dialogs ) 
add_subdirectory( pictures ) 
add_subdirectory( sounds ) 

include_directories(dialogs/ ${CMAKE_SOURCE_DIR}/libkdegames ${CMAKE_SOURCE_DIR}/libkdegames/highscore  )


########### next target ###############
kde4_add_ui3_files(libkibattleship_SRCS dialogs/connectDlg.ui dialogs/serverDlg.ui dialogs/chatDlg.ui dialogs/statDlg.ui dialogs/infoDlg.ui)

set(kbattleship_SRCS ${libkibattleship_SRCS}
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

kde4_automoc(${kbattleship_SRCS})

kde4_add_executable(kbattleship ${kbattleship_SRCS})

target_link_libraries(kbattleship  ${KDE4_KDEUI_LIBS} kdegames kdnssd )

install_targets(/bin kbattleship )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kbattleship.desktop )
install_files( ${DATA_INSTALL_DIR}/kbattleship FILES kbattleshipui.rc eventsrc )
install_files( ${DATA_INSTALL_DIR}/zeroconf FILES _kbattleship._tcp )




#original Makefile.am contents follow:

#SUBDIRS	= dialogs . pictures sounds
#
#INCLUDES = -I$(top_srcdir)/libkdegames -I$(top_srcdir)/libkdegames/highscore $(all_includes)
#METASOURCES = AUTO
#
#bin_PROGRAMS = kbattleship
#kbattleship_SOURCES = kbaiplayer.cpp kbstrategy.cpp kbverticalstepstrategy.cpp\
#				kbdestroyshipstrategy.cpp kbhorizontalstepstrategy.cpp \
#				kbrandomshotstrategy.cpp kbdiagonalwrapstrategy.cpp \
#				kmessage.cpp kbattleshipserver.cpp kbattleshipclient.cpp \
#				kbattleshipview.cpp kgridwidget.cpp kbattlefield.cpp \
#	            kchatwidget.cpp kserverdialog.cpp kclientdialog.cpp \
#				kstatdialog.cpp	kbdiagonalshotstrategy.cpp \
#	            konnectionhandling.cpp kship.cpp kshiplist.cpp \
#	            kbchooserstrategy.cpp kbattleship.cpp main.cpp
#kbattleship_LDADD = $(LIB_KDEGAMES) ./dialogs/libkbattleshipdialogs.la $(LIB_KFILE) $(LIB_KDNSSD)
#kbattleship_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kbattleship_DEPENDENCIES = $(LIB_KDEGAMES_DEP)
#
#services_DATA = _kbattleship._tcp
#servicesdir = $(kde_datadir)/zeroconf
#
#xdg_apps_DATA = kbattleship.desktop
#
#rcdir = $(kde_datadir)/kbattleship
#rc_DATA = kbattleshipui.rc eventsrc
#
#messages: rc.cpp
#	$(EXTRACTRC) */*.ui >> rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kbattleship.pot
