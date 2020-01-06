cmake_minimum_required(VERSION 3.5.1)
project(debugger)

set(SOURCE_FILES
  src/debugger_core.cpp
  src/cli_debugger.cpp
  )

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

include_directories(${MODULE_INCLUDE_DIRS} include)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(${PROJECT_NAME} cxxopts)
