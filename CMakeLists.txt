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

#Version numbers
set (HexGame_VERSION_MAJOR 0)
set (HexGame_VERSION_MINOR 95)

add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Release)
add_subdirectory(${PROJECT_SOURCE_DIR}/bin/Debug)
