cmake_minimum_required(VERSION 3.5.1)
project(gameboy)

set(SOURCE_FILES
    src/gameboy.cpp
)

include_directories(${MODULE_INCLUDE_DIRS})

add_library(gameboy STATIC ${SOURCE_FILES})

target_link_libraries(gameboy util cpu gpu video memory controller cartridge)

target_include_directories(gameboy PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
