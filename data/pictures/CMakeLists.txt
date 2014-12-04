
install(FILES
default.desktop
default_theme.svgz
DESTINATION ${DATA_INSTALL_DIR}/knavalbattle/pictures)

ecm_install_icons(ICONS
    16-apps-knavalbattle.png
    22-apps-knavalbattle.png
    32-apps-knavalbattle.png
    48-apps-knavalbattle.png
    64-apps-knavalbattle.png
    128-apps-knavalbattle.png
    DESTINATION ${ICON_INSTALL_DIR}
    THEME hicolor
)

