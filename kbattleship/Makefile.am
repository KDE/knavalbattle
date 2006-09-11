SUBDIRS	= dialogs . pictures sounds

INCLUDES = -I$(top_srcdir)/libkdegames -I$(top_srcdir)/libkdegames/highscore $(all_includes)
METASOURCES = AUTO

bin_PROGRAMS = kbattleship
kbattleship_SOURCES = kbaiplayer.cpp kbstrategy.cpp kbverticalstepstrategy.cpp\
				kbdestroyshipstrategy.cpp kbhorizontalstepstrategy.cpp \
				kbrandomshotstrategy.cpp kbdiagonalwrapstrategy.cpp \
				kmessage.cpp kbattleshipserver.cpp kbattleshipclient.cpp \
				kbattleshipview.cpp kgridwidget.cpp kbattlefield.cpp \
	            kchatwidget.cpp kserverdialog.cpp kclientdialog.cpp \
				kstatdialog.cpp	kbdiagonalshotstrategy.cpp \
	            konnectionhandling.cpp kship.cpp kshiplist.cpp \
	            kbchooserstrategy.cpp kbattleship.cpp main.cpp
kbattleship_LDADD = $(LIB_KDEGAMES) ./dialogs/libkbattleshipdialogs.la $(LIB_KFILE) $(LIB_KDNSSD)
kbattleship_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kbattleship_DEPENDENCIES = $(LIB_KDEGAMES_DEP)

services_DATA = _kbattleship._tcp
servicesdir = $(kde_datadir)/zeroconf

xdg_apps_DATA = kbattleship.desktop

rcdir = $(kde_datadir)/kbattleship
rc_DATA = kbattleshipui.rc eventsrc

messages: rc.cpp
	$(EXTRACTRC) */*.ui >> rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kbattleship.pot

kbattleship.o: dialogs/infoDlg.h dialogs/chatDlg.h dialogs/connectDlg.h dialogs/serverDlg.h dialogs/statDlg.h
kbattleshipview.o: dialogs/infoDlg.h dialogs/chatDlg.h dialogs/connectDlg.h dialogs/serverDlg.h dialogs/statDlg.h
kchatwidget.o: dialogs/chatDlg.h
kclientdialog.o: dialogs/connectDlg.h
kserverdialog.o: dialogs/serverDlg.h
kstatdialog.o: dialogs/statDlg.h

