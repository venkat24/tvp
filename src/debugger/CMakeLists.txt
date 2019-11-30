cmake_minimum_required(VERSION 3.5.1)
project(debugger)

set(SOURCE_FILES
    src/debugger.cpp
    src/disassembly_loader.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

include_directories(${MODULE_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
