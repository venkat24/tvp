cmake_minimum_required(VERSION 3.11.1)
project(main)

set(SOURCE_FILES
	# Main
	main/main.cpp
)

add_executable(tvp ${SOURCE_FILES})

install(TARGETS tvp
	RUNTIME DESTINATION bin
)
