cmake_minimum_required(VERSION 3.5.1)
project(gpu)

set(SOURCE_FILES
    src/gpu.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/util/include
    ${CMAKE_SOURCE_DIR}/src/cpu/include
    ${CMAKE_SOURCE_DIR}/src/memory/include
    ${CMAKE_SOURCE_DIR}/src/video/include
)

add_library(gpu STATIC ${SOURCE_FILES})

target_link_libraries(gpu util)

target_include_directories(gpu PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
