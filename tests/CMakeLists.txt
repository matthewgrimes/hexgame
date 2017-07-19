cmake_minimum_required (VERSION 2.8)

add_executable(testMesh testMesh.cpp)
target_link_libraries(testMesh UnitTest++ Mesh)

add_executable(testLighting testLighting.cpp)
target_link_libraries(testLighting UnitTest++ Lighting)

add_custom_command(TARGET testMesh
	POST_BUILD COMMAND testMesh
    	COMMENT "Running unit tests")

add_custom_command(TARGET testLighting
	POST_BUILD COMMAND testLighting
    	COMMENT "Running unit tests")

add_custom_target(UnitTests
	DEPENDS testMesh testLighting)

