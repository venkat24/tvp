cmake_minimum_required(VERSION 3.5.1)
project(memory)

set(SOURCE_FILES
    src/memory.cpp
)

add_library(memory STATIC ${SOURCE_FILES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src/cpu/include
    ${CMAKE_SOURCE_DIR}/src/gpu/include
    ${CMAKE_SOURCE_DIR}/src/cartridge/include
    ${CMAKE_SOURCE_DIR}/src/controller/include
    ${CMAKE_SOURCE_DIR}/src/timer/include
    ${CMAKE_SOURCE_DIR}/src/util/include
)

target_link_libraries(memory util)

target_include_directories(memory PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
