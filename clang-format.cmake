# Creates additional target to perform clang-format run, requires clang-format

# Find clang format
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
find_package(ClangFormat)

if(CLANG_FORMAT_FOUND)
	message("-- Found clang-format")
	message("-- clang-format executable location: ${CLANG_FORMAT_EXECUTABLE}")
	message("-- clang-format version: ${CLANG_FORMAT_VERSION}")
else()
	message(SEND_ERROR "clang-format executable not found")
endif()

# Get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h src/*.proto test/*.cpp test/*.h)

# Add target to build
add_custom_target(
	clangformat
	COMMAND ${CLANG_FORMAT_EXECUTABLE}
	-style=file
	-i
	${ALL_SOURCE_FILES}
)
