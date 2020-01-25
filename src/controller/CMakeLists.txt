cmake_minimum_required(VERSION 3.5.1)
project(controller)

set(SOURCE_FILES
	src/controller.cpp
	src/recording_controller.cpp
)

add_library(controller STATIC ${SOURCE_FILES})

include_directories(${MODULE_INCLUDE_DIRS})

target_include_directories(controller PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(controller jsoncpp)
