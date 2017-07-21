cmake_minimum_required (VERSION 2.8)

FILE(COPY ${PROJECT_SOURCE_DIR}/hexgame-dep/extern/lib/libwinpthread-1.dll DESTINATION ./)

if (${BUILDING_MINGW} EQUAL -1)
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
else()
	message(STATUS "Cross-compiling")
	add_executable(testMesh.exe testMesh.cpp)
	target_link_libraries(testMesh.exe UnitTest++ Mesh)

	add_executable(testLighting.exe testLighting.cpp)
	target_link_libraries(testLighting.exe UnitTest++ Lighting)

	message(STATUS "Running with wine")
	add_custom_command(TARGET testMesh.exe
		POST_BUILD COMMAND wine testMesh.exe
		COMMENT "Running unit tests")

	add_custom_command(TARGET testLighting.exe
		POST_BUILD COMMAND wine testLighting.exe
		COMMENT "Running unit tests")

	add_custom_target(UnitTests
		DEPENDS testMesh testLighting)

endif()

