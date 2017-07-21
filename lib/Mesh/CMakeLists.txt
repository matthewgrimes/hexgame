cmake_minimum_required (VERSION 2.8)

set (EXTERNAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/hexgame-dep/extern/include)
include_directories(${PROJECT_INCLUDE_DIR} ${EXTERNAL_INCLUDE_DIR})
add_library(Mesh Mesh.cpp)

