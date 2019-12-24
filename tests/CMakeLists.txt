cmake_minimum_required(VERSION 3.5.1)
project(tests)

include(GoogleTest)

include_directories(
	.
	${CMAKE_SOURCE_DIR}/src/cpu/include
	${CMAKE_SOURCE_DIR}/src/gpu/include
	${CMAKE_SOURCE_DIR}/src/memory/include
	${CMAKE_SOURCE_DIR}/src/util/include
)

set(SOURCE_FILES
	tests.cpp

	# CPU
	cpu/register_test.cpp
	#cpu/arithmetic_opcode_test.cpp
)

add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests cpu memory gpu gtest gmock)
gtest_add_tests(tests "" AUTO)

install(TARGETS tests
	RUNTIME DESTINATION bin
)
