cmake_minimum_required(VERSION 3.11.1)
project(cpu)

set(SOURCE_FILES
    src/cpu.cpp
    src/register/register.cpp
)

set(INCLUDE_PATH include)

add_library(cpu STATIC ${SOURCE_FILES})
target_link_libraries(cpu memory)

target_include_directories(cpu PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
)
