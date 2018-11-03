cmake_minimum_required(VERSION 3.11.1)
project(util)

set(SOURCE_FILES
    src/log.cpp
    src/helpers.cpp
)

set(INCLUDE_PATH include)

add_library(util STATIC ${SOURCE_FILES})

target_include_directories(util PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}>
)
