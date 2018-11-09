cmake_minimum_required(VERSION 3.11.1)
project(test)

include(GoogleTest)

include_directories(
	.
	${CMAKE_SOURCE_DIR}/src/cpu/include
	${CMAKE_SOURCE_DIR}/src/gpu/include
	${CMAKE_SOURCE_DIR}/src/memory/include
	${CMAKE_SOURCE_DIR}/src/util/include
)

set(SOURCE_FILES
	test.cpp

	# CPU
	cpu/register_test.cpp
	cpu/arithmetic_opcode_test.cpp
)

add_executable(test ${SOURCE_FILES})
target_link_libraries(test cpu memory gtest gmock)
gtest_add_tests(test "" AUTO)

install(TARGETS test
	RUNTIME DESTINATION bin
)
