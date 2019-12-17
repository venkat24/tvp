cmake_minimum_required(VERSION 3.5.1)
project(tvp_test)

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
	#cpu/arithmetic_opcode_test.cpp
)

add_executable(tvp_test ${SOURCE_FILES})
target_link_libraries(tvp_test cpu memory gpu gtest gmock)
gtest_add_tests(tvp_test "" AUTO)

install(TARGETS tvp_test
	RUNTIME DESTINATION bin
)
