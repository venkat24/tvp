cmake_minimum_required(VERSION 3.5.1)
project(cartridge)

set(SOURCE_FILES
    src/meta_cartridge.cpp
    src/cartridge.cpp
)

include_directories(${MODULE_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
