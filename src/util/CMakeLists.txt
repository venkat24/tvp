cmake_minimum_required(VERSION 3.5.1)
project(util)

set(SOURCE_FILES
    src/log.cpp
    src/helpers.cpp
    src/argv_generator.cpp)

include_directories(${MODULE_INCLUDE_DIRS})

add_library(util STATIC ${SOURCE_FILES})

target_include_directories(util PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
