cmake_minimum_required (VERSION 2.8)

set (CMAKE_BUILD_TYPE=Release)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set (LIBRARIES ${LIBRARIES} Mesh Lighting)

FILE ( GLOB_RECURSE HEXGAME_SOURCES ${PROJECT_SOURCE_DIR}/src/*cpp )


add_executable( HexGame.exe ${HEXGAME_SOURCES})
add_dependencies(HexGame.exe generate_version)

target_link_libraries(HexGame.exe ${LIBRARIES} ${EXTERNAL_LIBRARIES})

FILE(COPY ${PROJECT_SOURCE_DIR}/hexgame-data/data DESTINATION ./)

FILE(GLOB_RECURSE DLLFILES ${PROJECT_SOURCE_DIR}/hexgame-dep/dll/*.dll)
FILE(COPY ${DLLFILES} DESTINATION ./)
