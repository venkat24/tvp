cmake_minimum_required(VERSION 3.11.1)
project(main)

set(SOURCE_FILES main.cpp)

add_executable(main ${SOURCE_FILES})

install(TARGETS main
	RUNTIME DESTINATION bin
)
