cmake_minimum_required(VERSION 3.0)


#### Customize with your own environement
SET(VCPKG_BASE_PATH $ENV{EXTERNALS2_PATH}/windows/vs2019/VCPKG/)
####
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_BASE_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

#############################################
project(QtTools)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "My multi config types" FORCE)

IF(MSVC)
	set(CMAKE_DEBUG_POSTFIX D)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ELSE(MSVC)
	SET(CMAKE_CXX_FLAGS "-O3 -Wno-deprecated -Wno-invalid-offsetof -Wno-unused-result -fext-numeric-literals -Wno-psabi -fpermissive")
	SET(CMAKE_C_FLAGS "-O3 -Wno-deprecated -Wno-unused-result -Wno-psabi")
ENDIF(MSVC)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt
FIND_PACKAGE(Qt5Widgets REQUIRED)
IF(WIN32)
	FIND_PACKAGE(Qt5WinExtras REQUIRED)
ENDIF(WIN32)

SET(BROWSER_SRC
Browser/qtbuttonpropertybrowser.cpp
Browser/qtbuttonpropertybrowser.h
Browser/qteditorfactory.cpp
Browser/qteditorfactory.h
Browser/qtgroupboxpropertybrowser.cpp
Browser/qtgroupboxpropertybrowser.h
Browser/qtpropertybrowser.cpp
Browser/qtpropertybrowser.h
Browser/qtpropertybrowser.qrc
Browser/qtpropertybrowserutils.cpp
Browser/qtpropertybrowserutils_p.h
Browser/qtpropertymanager.cpp
Browser/qtpropertymanager.h
Browser/qttreepropertybrowser.cpp
Browser/qttreepropertybrowser.h
Browser/qtvariantproperty.cpp
Browser/qtvariantproperty.h
Browser/QtTools.h
)

SET(WINMIGRATE_SRC
QTWinMigrate/QMfcApp
QTWinMigrate/qmfcapp.cpp
QTWinMigrate/qmfcapp.h
QTWinMigrate/qtwinmigrate.pri
QTWinMigrate/QWinHost
QTWinMigrate/qwinhost.cpp
QTWinMigrate/qwinhost.h
QTWinMigrate/QWinWidget
QTWinMigrate/qwinwidget.cpp
QTWinMigrate/qwinwidget.h
)

add_library(QtTools SHARED
${BROWSER_SRC}
${WINMIGRATE_SRC}
)

target_include_directories(QtTools PRIVATE)

target_link_libraries(QtTools 
Qt5::Widgets
)

IF(WIN32)
    target_compile_definitions(QtTools PRIVATE QTTOOLS_EXPORTS QT_QTWINMIGRATE_EXPORT _CRT_SECURE_NO_WARNINGS)
ENDIF(WIN32)

install(TARGETS QtTools)
install(DIRECTORY "QTWinMigrate/" 
        DESTINATION "include/QtTools/QTWinMigrate"
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY "Browser/" 
        DESTINATION "include/QtTools/Browser"
        FILES_MATCHING PATTERN "*.h")
