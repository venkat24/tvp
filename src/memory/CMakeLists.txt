cmake_minimum_required(VERSION 3.5.1)
project(memory)

set(SOURCE_FILES
    src/memory.cpp
)

include_directories(${MODULE_INCLUDE_DIRS})

add_library(memory STATIC ${SOURCE_FILES})

target_link_libraries(memory util)

target_include_directories(memory PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
