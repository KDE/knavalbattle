
INCLUDES = $(all_includes)

noinst_LTLIBRARIES = libkbattleshipdialogs.la

libkbattleshipdialogs_la_SOURCES = dummy.cpp connectDlg.ui serverDlg.ui chatDlg.ui statDlg.ui
libkbattleshipdialogs_la_LIBADD = $(LIB_KDEUI)
libkbattleshipdialogs_la_LDFLAGS = $(all_libraries)
libkbattleshipdialogs_la_METASOURCES = AUTO

dummy.cpp:
	echo > dummy.cpp

DISTCLEANFILES = dummy.cpp

