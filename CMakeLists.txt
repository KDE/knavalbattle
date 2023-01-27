cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

set (RELEASE_SERVICE_VERSION_MAJOR "23")
set (RELEASE_SERVICE_VERSION_MINOR "03")
set (RELEASE_SERVICE_VERSION_MICRO "70")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")
set (RELEASE_SERVICE_COMPACT_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}${RELEASE_SERVICE_VERSION_MINOR}${RELEASE_SERVICE_VERSION_MICRO}")

project(knavalbattle VERSION "2.1.${RELEASE_SERVICE_COMPACT_VERSION}")

set (QT_MIN_VERSION "5.15.2")
set (KF_MIN_VERSION "5.91.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
else()
    set(KF_MAJOR_VERSION "5")
endif()
include(FeatureSummary)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets)

find_package(KF${KF_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS
  CoreAddons
  Config
  Crash
  DBusAddons
  DNSSD
  DocTools
  I18n
  TextWidgets
  XmlGui
)

find_package(KF5KDEGames 7.3.0 REQUIRED)

ecm_set_disabled_deprecation_versions(
    QT 6.4
    KF 5.91
    KDEGAMES 7.3
)

add_subdirectory(src) 
add_subdirectory(data)
add_subdirectory(doc)
ecm_qt_install_logging_categories(EXPORT KNAVALBATTLE FILE knavalbattle.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
