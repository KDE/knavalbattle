kde4_header()




########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#
#noinst_LTLIBRARIES = libkbattleshipdialogs.la
#libkbattleshipdialogs_la_SOURCES = dummy.cpp connectDlg.ui serverDlg.ui \
#	chatDlg.ui statDlg.ui infoDlg.ui 
#libkbattleshipdialogs_la_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#libkbattleshipdialogs_la_LIBADD = $(LIB_KDEUI)
#
#dummy.cpp:
#	echo > dummy.cpp
#
#DISTCLEANFILES = dummy.cpp
#
