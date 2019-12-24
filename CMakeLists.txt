cmake_minimum_required(VERSION 3.5.1)
project(tvp)

# Add the CMake folder to the module search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Force C++17 Standard
set(CMAKE_CXX_STANDARD 17)

# Set some flags for debug and release modes
if (NOT WIN32)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else()
	message(WARNING "Compiler flags may be set incorrectly on Windows")
endif()

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
  cartridge
  video
  controller
  gpu
  gameboy
  debugger
)

# Make list of all module header paths
foreach(MODULE ${MODULES})
  list(APPEND MODULE_INCLUDE_DIRS " ${CMAKE_SOURCE_DIR}/src/${MODULE}/include")
endforeach()

# Add all modules to build
foreach(MODULE ${MODULES})
  add_subdirectory(src/${MODULE})
endforeach(MODULE)

# I don't know why I need to do this here on Windows, but it woks if this is here
# TODO: Fix this. SFML variables shouldn't be needed outside the video module
if (WIN32)
	include_directories(${SFML_INCLUDE_DIR})
endif()

# Add the main executable
add_executable(tvp src/main/main.cpp)

# Link headers and libraries
target_link_libraries(tvp cxxopts)
target_link_libraries(tvp ${MODULES})

# Add dependencies
add_subdirectory(ext/cxxopts)

# If this is not the release build, compile unit tests
# TODO: This is should be triggered by some other flag, not CMAKE_BUILD_TYPE
if((NOT CMAKE_BUILD_TYPE STREQUAL "Release"))
	if (NOT WIN32)
		include(cmake/clang-format.cmake)
		add_subdirectory(ext/googletest)
		add_subdirectory(tests)
	else()
		message(WARNING "No Clang-Format support on Windows")
		message(WARNING "No Unit Testing support on Windows")
	endif()
endif()

# Set install destinations
install(TARGETS tvp
	RUNTIME DESTINATION bin
)
