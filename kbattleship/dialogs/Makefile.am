
INCLUDES = $(all_includes)

noinst_LTLIBRARIES = libkbattleshipdialogs.la

libkbattleshipdialogs_la_SOURCES = dummy.cc connectDlg.ui serverDlg.ui
libkbattleshipdialogs_la_LIBADD = $(LIB_KDEUI)
libkbattleshipdialogs_la_LDFLAGS = $(all_libraries)
libkbattleshipdialogs_la_METASOURCES = AUTO

dummy.cc:
	echo > dummy.cc

DISTCLEANFILES = dummy.cc

