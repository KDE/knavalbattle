INCLUDES                    =   $(all_includes) -I$(top_builddir)/libkdegames
SUBDIRS	                    =   dialogs . pictures sounds

lib_LTLIBRARIES  	    =	kbattleship.la

kbattleship_la_SOURCES      =   kbaiplayer.cpp kbstrategy.cpp kbverticalstepstrategy.cpp \
				kbdestroyshipstrategy.cpp kbhorizontalstepstrategy.cpp \
				kbrandomshotstrategy.cpp \
				kmessage.cpp kbattleshipserver.cpp kbattleshipclient.cpp \
				kbattleshipview.cpp kgridwidget.cpp kbattlefield.cpp \
		                kchatwidget.cpp kserverdialog.cpp kclientdialog.cpp \
				kstatdialog.cpp ksingledialog.cpp khighscoredialog.cpp \
				kbattleshipsound.cpp \
		                konnectionhandling.cpp kship.cpp kshiplist.cpp \
		                kbattleship.cpp main.cpp

kbattleship_la_LIBADD       =   $(top_builddir)/libkdegames/libkdegames.la \
				dialogs/libkbattleshipdialogs.la $(LIB_KFILE) -lsoundserver_idl
kbattleship_la_LDFLAGS      =   -avoid-version -module $(all_libraries)

bin_PROGRAMS                =   kbattleship
kbattleship_LDFLAGS	    =	$(KDE_RPATH) $(all_libraries)
kbattleship_LDADD	    =	kbattleship.la
kbattleship_SOURCES	    =	dummy.cpp

METASOURCES                 =   AUTO
CLEANFILES		    =	dummy.cpp

dummy.cpp:
	echo > dummy.cpp

rcdir                       =   $(kde_datadir)/kbattleship
rc_DATA                     =   kbattleshipui.rc

kdelnkdir                   =   $(kde_appsdir)/Games
kdelnk_DATA                 =   kbattleship.desktop

messages: rc.cpp
	$(EXTRACTRC) */*.ui >> rc.cpp
	$(XGETTEXT) rc.cpp *.cpp main.h -o $(podir)/kbattleship.pot
