macro(ee_enable_clang_tidy)

	find_program(CLANGTIDY clang-tidy)

	if(CLANGTIDY)
		set(CLANG_TIDY_OPTIONS
			${CLANGTIDY}
			--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
			-p=${CMAKE_BINARY_DIR}
		)

		if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
			list(APPEND CLANG_TIDY_OPTIONS
                -extra-arg=-std=c++${CMAKE_CXX_STANDARD}
            )
		endif()

		if(ee_WARNINGS_AS_ERRORS)
			list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
		endif()

		set_property(TARGET ee_project_options PROPERTY EE_CLANG_TIDY ${CLANG_TIDY_OPTIONS})

		message(${WARNING_MESSAGE} "clang-tidy set up")
	else()
		message(${WARNING_MESSAGE} "clang-tidy requested but executable not found")
	endif()

endmacro()
