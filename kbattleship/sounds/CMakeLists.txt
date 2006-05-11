kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kbattleship/sounds FILES ship-player-shoot-water.ogg  	    ship-player1-shoot.ogg  	    ship-player2-shoot.ogg  	    ship-sink.ogg )

kde4_footer()



#original Makefile.am contents follow:

#sounddir = $(kde_datadir)/kbattleship/sounds
#sound_DATA = ship-player-shoot-water.ogg \
#	    ship-player1-shoot.ogg \
#	    ship-player2-shoot.ogg \
#	    ship-sink.ogg
