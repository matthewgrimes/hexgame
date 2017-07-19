cmake_minimum_required (VERSION 2.8)

set (CMAKE_BUILD_TYPE=Debug)

FILE ( GLOB_RECURSE HEXGAME_SOURCES ${PROJECT_SOURCE_DIR}/src/*cpp )

add_executable( HexGame-debug.exe ${HEXGAME_SOURCES})
add_dependencies(HexGame-debug.exe generate_version cppcheck UnitTests)

set( LIBRARIES Mesh Lighting )

target_link_libraries(HexGame-debug.exe ${LIBRARIES} ${EXTERNAL_LIBRARIES})

FILE(COPY ${PROJECT_SOURCE_DIR}/hexgame-data/data DESTINATION ./)

FILE(GLOB_RECURSE DLLFILES ${PROJECT_SOURCE_DIR}/hexgame-dep/dll/*.dll)
FILE(COPY ${DLLFILES} DESTINATION ./)
