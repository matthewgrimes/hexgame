set (CMAKE_SYSTEM_NAME Generic)
set (CMAKE_CROSSCOMPILING 1)
set (CMAKE_SYSTEM_PROCESSOR arm)

set (CMAKE_C_COMPILER /usr/bin/i686-w64-mingw32-gcc-win32)
set (CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-g++-win32)

set (DLLTOOL /usr/bin/i686-w64-mingw32-dlltool)

set (CMAKE_FIND_ROOT_PATH /usr/i686-mingw32)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
set (EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} mingw32)
set (EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} assimp glew32 OpenGL32 glu32 SDL2main SDL2 SDL2_image CEGUIBase-0 CEGUIOpenGLRenderer-0)
set (EXTERNAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/hexgame-dep/extern/include)
