cmake_minimum_required (VERSION 2.8)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set (EXTERNAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/hexgame-dep/extern/include)
set (EXTERNAL_LIB_DIR ${PROJECT_SOURCE_DIR}/hexgame-dep/extern/lib)
set (LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set (CMAKE_BUILD_TYPE=Release)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -static-libgcc -static-libstdc++ -mwindows -O3")

set (EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} mingw32 assimp glew32 OpenGL32 glu32 SDL2main SDL2 SDL2_image CEGUIBase-0 CEGUIOpenGLRenderer-0)

set (LIBRARIES ${LIBRARIES} Mesh Lighting)

include_directories(${PROJECT_INCLUDE_DIR} ${EXTERNAL_INCLUDE_DIR})
link_directories(${EXTERNAL_LIB_DIR} ${LIB_DIR})

FILE ( GLOB_RECURSE HEXGAME_SOURCES ${PROJECT_SOURCE_DIR}/src/*cpp )


add_executable( HexGame.exe ${HEXGAME_SOURCES})
add_dependencies(HexGame.exe generate_version)

target_link_libraries(HexGame.exe ${LIBRARIES} ${EXTERNAL_LIBRARIES})

FILE(COPY ${PROJECT_SOURCE_DIR}/hexgame-data/data DESTINATION ./)

FILE(GLOB_RECURSE DLLFILES ${PROJECT_SOURCE_DIR}/hexgame-dep/dll/*.dll)
FILE(COPY ${DLLFILES} DESTINATION ./)
