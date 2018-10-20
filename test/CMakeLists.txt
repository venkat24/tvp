cmake_minimum_required(VERSION 3.11.1)
project(test)

include(GoogleTest)

set(SOURCE_FILES
	test.cpp
)

add_executable(test ${SOURCE_FILES})
target_link_libraries(test gtest gmock)
gtest_add_tests(test "" AUTO)

install(TARGETS test
	RUNTIME DESTINATION bin
)
