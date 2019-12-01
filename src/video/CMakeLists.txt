cmake_minimum_required(VERSION 3.5.1)
project(video)

# Build static libs in Windows
if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)

    # This lets the user override SFML_ROOT
    cmake_policy(SET CMP0074 NEW)
endif()

find_package(SFML 2 REQUIRED graphics window system)

set(SOURCE_FILES
    src/video.cpp
)

include_directories(${SFML_INCLUDE_DIR} ${MODULE_INCLUDE_DIRS})

add_library(video STATIC ${SOURCE_FILES})

target_link_libraries(video util ${SFML_DEPENDENCIES} ${SFML_LIBRARIES})

target_include_directories(video PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
