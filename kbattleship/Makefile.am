INCLUDES                    =   $(all_includes)
SUBDIRS	                    =   dialogs . pictures sounds

lib_LTLIBRARIES  	    =	kbattleship.la

kbattleship_la_SOURCES      =   kmessage.cpp kbattleshipserver.cpp kbattleshipclient.cpp \
				kbattleshipview.cpp kgridwidget.cpp kbattlefield.cpp \
		                kchatwidget.cpp kserverdialog.cpp kclientdialog.cpp \
				kstatdialog.cpp kbattleshipsound.cpp \
		                konnectionhandling.cpp kship.cpp kshiplist.cpp \
		                kbattleship.cpp 

kbattleship_la_LIBADD       =   dialogs/libkbattleshipdialogs.la $(LIB_KFILE) -lsoundserver_idl
kbattleship_la_LDFLAGS      =   -module $(KDE_PLUGIN) $(all_libraries)

bin_PROGRAMS                =   kbattleship
kbattleship_LDFLAGS	    =	$(KDE_RPATH) $(all_libraries)
kbattleship_LDADD	    =	kbattleship.la
kbattleship_SOURCES	    =	main.cpp dummy.cpp

METASOURCES                 =   AUTO

CLEANFILES		    =	dummy.cpp

dummy.cpp:
	echo > dummy.cpp

rcdir                       =   $(kde_datadir)/kbattleship
rc_DATA                     =   kbattleshipui.rc

kdelnkdir                   =   $(kde_appsdir)/Games
kdelnk_DATA                 =   kbattleship.desktop

messages: rc.cpp
	$(XGETTEXT) rc.cpp *.cpp */*.cpp -o $(podir)/kbattleship.pot
