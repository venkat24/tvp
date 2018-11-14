cmake_minimum_required(VERSION 3.11.1)
project(tvp)

# Add the CMake folder to the module search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Force C++17 Standard
set(CMAKE_CXX_STANDARD 17)

# Set some flags for debug and release modes
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Copy the compile_commands.json into the root dir
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
  execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
endif()

# Always enable colors, since Ninja strips them
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
endif ()

# Binary output directory after build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# List of modules to add and compile
set(MODULES
  util
  memory
  cpu
  gpu
  cartridge
  video
)

foreach(MODULE ${MODULES})
  add_subdirectory(src/${MODULE})
endforeach(MODULE)

# Add the main executable
add_executable(tvp src/main/main.cpp)

# Link headers and libraries
target_link_libraries(tvp ${MODULES})

# If this is not the release build, compile unit tests
# TODO: This is should be triggered by some other flag, not CMAKE_BUILD_TYPE
if((NOT CMAKE_BUILD_TYPE STREQUAL "Release"))
	include(cmake/clang-format.cmake)
	add_subdirectory(ext/googletest)
	add_subdirectory(test)
endif()

# Set install destinations
install(TARGETS tvp
	RUNTIME DESTINATION bin
)
