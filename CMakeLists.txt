cmake_minimum_required(VERSION 3.11.1)
project(tvp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(ext/googletest)
add_subdirectory(test)
