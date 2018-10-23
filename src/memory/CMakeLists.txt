cmake_minimum_required(VERSION 3.11.1)
project(memory)

set(SOURCE_FILES
    src/memory.cpp
)

set(INCLUDE_PATH include)

add_library(memory STATIC ${SOURCE_FILES})

target_include_directories(memory PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
)
