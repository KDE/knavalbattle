INCLUDES = $(all_includes)
LDFLAGS = $(all_libraries) $(KDE_RPATH)
LIBADD = $(LIB_KDEUI)
METASOURCES = AUTO

noinst_LTLIBRARIES = libkbattleshipdialogs.la
libkbattleshipdialogs_la_SOURCES = dummy.cpp connectDlg.ui serverDlg.ui \
	chatDlg.ui statDlg.ui singleDlg.ui infoDlg.ui highscoreDlg.ui

dummy.cpp:
	echo > dummy.cpp

DISTCLEANFILES = dummy.cpp

