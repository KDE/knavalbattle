bin_PROGRAMS = kbattleship
INCLUDES         = $(all_includes)
SUBDIRS	= dialogs . pictures sounds
kbattleship_LDFLAGS = $(all_libraries)
kbattleship_LDADD   = dialogs/libkbattleshipdialogs.la $(LIB_KFILE)
kbattleship_SOURCES = kmessagetype.cpp kmessage.cpp kbattleshipserver.cpp kbattleshipclient.cpp \
                      kbattleshipview.cpp kgridwidget.cpp kbattlefieldtype.cpp \
		      kbattlefield.cpp kbattleship.cpp main.cpp
noinst_HEADERS   = kmessagetype.h kmessage.h kbattleshipserver.h kbattleshipclient.h \
                   kbattleshipview.h kbattleship.h kgridwidget.h kbattlefieldtype.h \
		   kbattlefield.h
METASOURCES = AUTO
KDE_ICON = kbattleship

rcdir = $(kde_datadir)/kbattleship
rc_DATA = kbattleshipui.rc

kdelnkdir   = $(kde_appsdir)/Games
kdelnk_DATA = KBattleship.desktop
