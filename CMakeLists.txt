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

# Set variable for the generate sources
set(GENERATE_SRCS ${PROJECT_SOURCE_DIR}/src/generate.c)
set(GENERATE_OUTPUT ${PROJECT_SOURCE_DIR}/generate)
set(VERSION_OUTPUT ${PROJECT_SOURCE_DIR}/include/Version.h)
set(VERSION_META "-alpha")

# Create command to compile the generate command
add_custom_command(
    OUTPUT ${GENERATE_OUTPUT}
    COMMAND gcc -o ${GENERATE_OUTPUT} ${GENERATE_SRCS}
    DEPENDS ${GENERATE_SRCS}
    )

add_custom_target(generate_version
    COMMAND ${GENERATE_OUTPUT}
        -M ${VERSION_MAJOR}
	-m ${VERSION_MINOR}
	-p ${VERSION_PATCH}
	-a "${VERSION_META}"
	-F "${PROJECT_SOURCE_DIR}/include/buildnumber"
	-i
	> ${VERSION_OUTPUT}
    DEPENDS ${GENERATE_OUTPUT}
    VERBATIM
    )
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

#Configure version header file
configure_file (
	"${CMAKE_SOURCE_DIR}/Version.h.in"
	"${PROJECT_INCLUDE_DIR}/Version.h"
)

add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Release)
add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Debug)
