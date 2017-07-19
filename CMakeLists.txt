cmake_minimum_required (VERSION 2.8)

include (clang-format.cmake)
include (cppcheck.cmake)

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

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -static-libgcc -static-libstdc++ -O3")
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (EXTERNAL_LIB_DIR ${PROJECT_SOURCE_DIR}/hexgame-dep/extern/lib)
set (LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_INCLUDE_DIR} ${EXTERNAL_INCLUDE_DIR})
link_directories(${EXTERNAL_LIB_DIR} ${LIB_DIR})

string(FIND "${CMAKE_CXX_COMPILER}" "mingw" BUILDING_MINGW)
if (${BUILDING_MINGW} EQUAL -1)
message(STATUS "Assuming building for Linux with ${CMAKE_CXX_COMPILER}")
set (EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} assimp GLEW GL GLU SDL2-2.0 SDL2_image-2.0 CEGUIBase-0 CEGUIOpenGLRenderer-0)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Release)
add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Debug)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)

