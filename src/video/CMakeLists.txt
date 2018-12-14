cmake_minimum_required(VERSION 3.11.1)
project(video)

find_package(SFML 2 REQUIRED graphics window system)

set(SOURCE_FILES
    src/video.cpp
)

include_directories(
    ${SFML_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/util/include
    ${CMAKE_SOURCE_DIR}/src/cpu/include
    ${CMAKE_SOURCE_DIR}/src/gpu/include
    ${CMAKE_SOURCE_DIR}/src/memory/include
    ${CMAKE_SOURCE_DIR}/src/controller/include
)

add_library(video STATIC ${SOURCE_FILES})

target_link_libraries(video util ${SFML_LIBRARIES})

target_include_directories(video PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
