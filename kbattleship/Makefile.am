bin_PROGRAMS = kbattleship
INCLUDES         = $(all_includes)
SUBDIRS	= dialogs . pictures sounds
kbattleship_LDFLAGS = $(all_libraries)
kbattleship_LDADD   = dialogs/libkbattleshipdialogs.la $(LIB_KFILE) -lsoundserver_idl
kbattleship_SOURCES = kmessage.cpp kbattleshipserver.cpp kbattleshipclient.cpp \
                      kbattleshipview.cpp kgridwidget.cpp kbattlefieldtype.cpp \
		      kbattlefield.cpp chatwidget.ui kchatwidget.cpp \
		      kserverdialog.cpp kclientdialog.cpp kbattleshipsound.cpp \
		      konnectionhandling.cpp kshiptype.cpp kship.cpp kshiplist.cpp \
		      kbattleship.cpp main.cpp
noinst_HEADERS   = kmessage.h kbattleshipserver.h kbattleshipclient.h \
                   kbattleshipview.h kgridwidget.h kbattlefieldtype.h \
		   kbattlefield.h chatwidget.h kchatwidget.h \
		   kserverdialog.h kclientdialog.h kbattleshipsound.h \
		   konnectionhandling.h kshiptype.h kship.h kshiplist.h kbattleship.h
METASOURCES = AUTO

rcdir = $(kde_datadir)/kbattleship
rc_DATA = kbattleshipui.rc

kdelnkdir   = $(kde_appsdir)/Games
kdelnk_DATA = kbattleship.desktop
