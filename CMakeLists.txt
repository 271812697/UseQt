cmake_minimum_required(VERSION 3.8)
project(UseQt CXX)

option(USE_QT6 "Build with Qt6 (Enabled by default)" ON)

if(USE_QT6)
  find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
else()
  find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets)
message(STATUS "QT_VERSION: ${QT_VERSION}, QT_DIR: ${QT_DIR}")

if (${QT_VERSION} VERSION_LESS 5.11.0)
  message(FATAL_ERROR "Requires qt version >= 5.11.0, Your current version is ${QT_VERSION}")
endif()

# Unfortunately, as we have a split include/src, AUTOMOC doesn't work.
# We'll have to manually specify some files
SET_PROPERTY(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP auto_gen)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


add_subdirectory(Block)
add_subdirectory(RayTrace)
add_subdirectory(QtSunEarth)
add_subdirectory(DirectX11Init)
add_subdirectory(ScreenWidget)
add_subdirectory(opengl)