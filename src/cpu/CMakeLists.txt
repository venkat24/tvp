cmake_minimum_required(VERSION 3.5.1)
project(cpu)

set(SOURCE_FILES
    src/cpu.cpp
    src/opcodes.cpp
    src/register/register.cpp
)

include_directories(${MODULE_INCLUDE_DIRS})

add_library(cpu STATIC ${SOURCE_FILES})

target_link_libraries(cpu util)

target_include_directories(cpu PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
