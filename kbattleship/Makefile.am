## Makefile.am for kmakefile's

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = 

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kbattleship

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
kbattleship_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kbattleship_LDADD   = -lkdecore -lkdeui -lqt

# which sources should be compiled for kmakefile's
kbattleship_SOURCES = kmessagetype.cpp kmessage.cpp kbattleshipserver.cpp

# these are the headers for your project
noinst_HEADERS   = kmessagetype.h kmessage.h kbattleshipserver.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
KDE_ICON = kbattleship

 # this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Games
kdelnk_DATA = KBattleship.desktop
