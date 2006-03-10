kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kbattleship/sounds FILES ship-player-shoot-water.ogg  	    ship-player1-shoot.ogg  	    ship-player2-shoot.ogg  	    ship-sink.ogg )

kde4_footer()



#original Makefile.am contents follow:

#sounddir = $(kde_datadir)/kbattleship/sounds
#sound_DATA = ship-player-shoot-water.ogg \
#	    ship-player1-shoot.ogg \
#	    ship-player2-shoot.ogg \
#	    ship-sink.ogg
