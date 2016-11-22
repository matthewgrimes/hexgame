cmake_minimum_required (VERSION 2.8)

if(UNIX)
set (CMAKE_SYSTEM_NAME Generic)
set (CMAKE_CROSSCOMPILING 1)
set (CMAKE_SYSTEM_PROCESSOR arm)
set (DLLTOOL /usr/bin/i686-w64-mingw32-dlltool)

set (CMAKE_C_COMPILER /usr/bin/i686-w64-mingw32-gcc-win32)
set (CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-g++-win32)
endif()

project( HexGame.exe )

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#
# Make a version file containing the current version from git.
#
include(GetGitRevisionDescription)
git_describe(VERSION --tags)

#parse the version information into pieces.
string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
#string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

#Version numbers
set (HexGame_VERSION_MAJOR ${VERSION_MAJOR})
set (HexGame_VERSION_MINOR ${VERSION_MINOR})
set (HexGame_VERSION_PATH ${VERSION_PATCH})

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

#Configure version header file
configure_file (
	"${CMAKE_SOURCE_DIR}/Version.h.in"
	"${PROJECT_INCLUDE_DIR}/Version.h"
)

add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Release)
add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Debug)
