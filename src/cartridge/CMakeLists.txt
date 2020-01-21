cmake_minimum_required(VERSION 3.5.1)
project(cartridge)

set(SOURCE_FILES
    src/cartridge_metadata.cpp
    src/cartridge.cpp
    src/instruction_parser.cpp
)

include_directories(${MODULE_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/ext/span/include>
)