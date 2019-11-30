cmake_minimum_required(VERSION 3.5.1)
project(debugger)

# Find Python interpreter libs, for calling Python scripts
find_package(PythonLibs REQUIRED)

set(SOURCE_FILES
    src/debugger.cpp
    src/disassembly_loader.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(debugger ${PYTHON_LIBRARIES})

include_directories(${PYTHON_INCLUDE_DIRS} ${MODULE_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)