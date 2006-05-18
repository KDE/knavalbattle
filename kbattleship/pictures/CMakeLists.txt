



########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kbattleship/pictures FILES
sea.png water.png hit.png
death.png border.png ship1-1.png
ship2-1.png ship2-2.png ship3-1.png
ship3-2.png ship3-3.png ship4-1.png
ship4-2.png ship4-3.png ship4-4.png
ship1-1-r.png ship2-1-r.png ship2-2-r.png
ship3-1-r.png ship3-2-r.png ship3-3-r.png
ship4-1-r.png ship4-2-r.png ship4-3-r.png
ship4-4-r.png stat-hits.png stat-shots.png stat-water.png )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )




#original Makefile.am contents follow:

#picsdir = $(kde_datadir)/kbattleship/pictures
#pics_DATA = sea.png \
#	    water.png \
#	    hit.png \
#	    death.png \
#	    border.png \
#	    ship1-1.png \
#	    ship2-1.png \
#	    ship2-2.png \
#	    ship3-1.png \
#	    ship3-2.png \
#	    ship3-3.png \
#	    ship4-1.png \
#	    ship4-2.png \
#	    ship4-3.png \
#	    ship4-4.png \
#	    ship1-1-r.png \
#	    ship2-1-r.png \
#	    ship2-2-r.png \
#	    ship3-1-r.png \
#	    ship3-2-r.png \
#	    ship3-3-r.png \
#	    ship4-1-r.png \
#	    ship4-2-r.png \
#	    ship4-3-r.png \
#	    ship4-4-r.png
#
#KDE_ICON = kbattleship
#
#EXTRA_DIST = $(pics_DATA)
