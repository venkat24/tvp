cmake_minimum_required(VERSION 3.11.1)
project(tvp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/memory)
add_subdirectory(src/cpu)

add_executable(tvp src/main/main.cpp)
target_link_libraries(tvp memory cpu)

install(TARGETS tvp
	RUNTIME DESTINATION bin
)

if((NOT CMAKE_BUILD_TYPE STREQUAL "Release"))
	include(clang-format.cmake)
	add_subdirectory(ext/googletest)
	add_subdirectory(test)
endif()
