set (CMAKE_SYSTEM_NAME Generic)
set (CMAKE_CROSSCOMPILING 1)
set (CMAKE_SYSTEM_PROCESSOR arm)

set (CMAKE_C_COMPILER /usr/bin/i686-w64-mingw32-gcc-win32)
set (CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-g++-win32)
set (CMAKE_RANLIB /usr/bin/i686-w64-mingw32-ranlib)
set (CMAKE_COMPILER_PREFIX "i686-w64-mingw32")

set (DLLTOOL /usr/bin/i686-w64-mingw32-dlltool)

set (CMAKE_FIND_ROOT_PATH /usr/i686-mingw32)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


