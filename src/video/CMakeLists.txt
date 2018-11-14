cmake_minimum_required(VERSION 3.11.1)
project(video)

set(SOURCE_FILES
    src/video.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/util/include
    ${CMAKE_SOURCE_DIR}/src/cpu/include
    ${CMAKE_SOURCE_DIR}/src/gpu/include
    ${CMAKE_SOURCE_DIR}/src/memory/include
)

add_library(video STATIC ${SOURCE_FILES})

target_link_libraries(video util)

target_include_directories(video PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
