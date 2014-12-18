project(knavalbattle)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.2.0")

find_package(ECM 1.3.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Svg)
find_package(KF5 REQUIRED COMPONENTS 
  CoreAddons 
  Config 
  WidgetsAddons 
  Config 
  DBusAddons 
  DNSSD 
  Declarative
  I18n 
  GuiAddons 
  Service 
  ConfigWidgets 
  ItemViews 
  XmlGui 
  KIO 
  NotifyConfig 
  NewStuff 
  KDELibs4Support)
    
find_package(KF5KDEGames 4.9.0 REQUIRED)

find_package(Phonon4Qt5 REQUIRED)

include_directories(BEFORE ${PHONON_INCLUDES})

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory(src) 
add_subdirectory(data) 
add_subdirectory(doc)
add_subdirectory(kconf_update)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
