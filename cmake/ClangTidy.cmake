macro(ee_enable_clang_tidy)

	find_program(CLANGTIDY clang-tidy)

	if(CLANGTIDY)
		set(CLANG_TIDY_OPTIONS
			${CLANGTIDY}
			-extra-arg=-Wno-unknown-warning-option
			-extra-arg=-Wno-ignored-optimization-argument
			-extra-arg=-Wno-unused-command-line-argument
			-p)

		if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
			if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
				set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
			else()
				set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
			endif()
		endif()

		if(ee_WARNINGS_AS_ERRORS)
			list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
		endif()

		set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})

		message(${WARNING_MESSAGE} "clang-tidy set up")
	else()
		message(${WARNING_MESSAGE} "clang-tidy requested but executable not found")
	endif()

endmacro()
