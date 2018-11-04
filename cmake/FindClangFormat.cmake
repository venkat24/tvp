# Utility to find clang-format

string(REPLACE ":" ";" _PATH $ENV{PATH})

foreach(p ${_PATH})
	file(GLOB cand ${p}/clang-format-[0-9]* ${p}/clang-format)
	if(cand)
			set(CLANG_FORMAT_EXECUTABLE ${cand})
			set(CLANG_FORMAT_FOUND ON)
			execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} -version OUTPUT_VARIABLE clang_out )
			string(REGEX MATCH .*\(version[^\n]*\)\n version ${clang_out})
			set(CLANG_FORMAT_VERSION ${CMAKE_MATCH_1})
			break()
	else()
			set(CLANG_FORMAT_FOUND OFF)
	endif()

endforeach()
