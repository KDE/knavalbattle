SUBDIRS	= dialogs . pictures sounds

INCLUDES = -I$(top_srcdir)/libkdegames -I$(top_srcdir)/libkdegames/highscore $(all_includes)
LDFLAGS = $(all_libraries) $(KDE_RPATH)
LDADD = $(LIB_KDEGAMES) ./dialogs/libkbattleshipdialogs.la $(LIB_KFILE) -lsoundserver_idl
METASOURCES = AUTO

bin_PROGRAMS = kbattleship
kbattleship_SOURCES = kbaiplayer.cpp kbstrategy.cpp kbverticalstepstrategy.cpp\
				kbdestroyshipstrategy.cpp kbhorizontalstepstrategy.cpp \
				kbrandomshotstrategy.cpp kbdiagonalwrapstrategy.cpp \
				kmessage.cpp kbattleshipserver.cpp kbattleshipclient.cpp \
				kbattleshipview.cpp kgridwidget.cpp kbattlefield.cpp \
	            kchatwidget.cpp kserverdialog.cpp kclientdialog.cpp \
				kstatdialog.cpp ksingledialog.cpp khighscoredialog.cpp \
				kbattleshipsound.cpp kbdiagonalshotstrategy.cpp \
	            konnectionhandling.cpp kship.cpp kshiplist.cpp \
	            kbchooserstrategy.cpp kbattleship.cpp main.cpp

xdg_apps_DATA = kbattleship.desktop

EXTRA_DIST = $(xdg_apps_DATA)

rcdir = $(kde_datadir)/kbattleship
rc_DATA = kbattleshipui.rc

messages: rc.cpp
	$(EXTRACTRC) */*.ui >> rc.cpp
	$(XGETTEXT) *.cpp main.h -o $(podir)/kbattleship.pot
