cmake_minimum_required(VERSION 3.0)

################################### ENV CHECK
IF(NOT DEFINED ENV{EXTERNALS2_PATH} AND NOT DEFINED ENV{EXTERNALS2_PATH_SYS})
	message( FATAL_ERROR "$EXTERNALS2_PATH or EXTERNALS2_PATH_SYS are not defined. Aborting" )
ENDIF()

### One system only env var (Windows or linux)
IF(DEFINED ENV{EXTERNALS2_PATH_SYS})
	SET(EXTERNALS_BASE_PATH "$ENV{EXTERNALS2_PATH_SYS}" CACHE STRING "")
ELSE()
	IF(WIN32)
		SET(PLATFORM "windows/vs2019" CACHE STRING "")
	ELSE(WIN32)
		SET(PLATFORM "linux/${LSB_DISTRIBUTOR_ID}/${LSB_RELEASE}" CACHE STRING "")
	ENDIF()

	SET(EXTERNALS_BASE_PATH "$ENV{EXTERNALS2_PATH}/${PLATFORM}" CACHE STRING "")
ENDIF()

file(TO_CMAKE_PATH "${EXTERNALS_BASE_PATH}" EXTERNALS_BASE_PATH)

message(STATUS "EXTERNALS2 BASE PATH: ${EXTERNALS_BASE_PATH}")

# Include toolchain from vcpkg
IF(WIN32)
	if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
		if(DEFINED ENV{VCPKGAVS_ROOT})
			set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKGAVS_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
		else()
			set(CMAKE_TOOLCHAIN_FILE "${EXTERNALS_BASE_PATH}/VCPKG/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
		endif()
	endif()
	message(STATUS "Using CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})
ENDIF(WIN32)

if(WIN32)
	STRING(APPEND EXTERNALS_BASE_PATH "/x64" ) #TODO x64/x86
else(WIN32)
	set(VCPKG_TARGET_TRIPLET "x64-linux-dynamic" CACHE STRING "VCPKG Target Triplet to use")
endif(WIN32)

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

IF(WIN32)
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
QTWinMigrate/QMfcApp
QTWinMigrate/QWinHost
QTWinMigrate/QWinWidget
)

set(CMAKE_MFC_FLAG 2)

ENDIF(WIN32)

add_library(QtTools SHARED
${BROWSER_SRC}
${WINMIGRATE_SRC}
)

target_include_directories(QtTools PRIVATE)

target_link_libraries(QtTools 
Qt5::Widgets
)

IF(WIN32)
    target_compile_definitions(QtTools PRIVATE QT_DLL QTTOOLS_EXPORTS QT_QTWINMIGRATE_EXPORT _CRT_SECURE_NO_WARNINGS _AFXDLL)
ENDIF(WIN32)

install(TARGETS QtTools)

IF(WIN32)
install(DIRECTORY "QTWinMigrate/" 
        DESTINATION "include/QtTools/QTWinMigrate"
        FILES_MATCHING PATTERN "*.h" PATTERN "QMfcApp" PATTERN "QWinHost" PATTERN "QWinWidget"
)
ENDIF(WIN32)

install(DIRECTORY "Browser/" 
        DESTINATION "include/QtTools/Browser"
        FILES_MATCHING PATTERN "*.h")
