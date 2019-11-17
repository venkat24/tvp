cmake_minimum_required(VERSION 3.5.1)
project(gameboy)

set(SOURCE_FILES
    src/gameboy.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/util/include
    ${CMAKE_SOURCE_DIR}/src/video/include
    ${CMAKE_SOURCE_DIR}/src/gpu/include
    ${CMAKE_SOURCE_DIR}/src/cpu/include
    ${CMAKE_SOURCE_DIR}/src/controller/include
    ${CMAKE_SOURCE_DIR}/src/cartridge/include
    ${CMAKE_SOURCE_DIR}/src/memory/include
)

add_library(gameboy STATIC ${SOURCE_FILES})

target_link_libraries(gameboy cpu gpu video memory controller cartridge)

target_include_directories(gameboy PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
