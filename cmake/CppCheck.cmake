macro(ee_enable_cppcheck)

	find_program(CPPCHECK cppcheck)

	if(CPPCHECK)
		set(CPPCHECK_TEMPLATE "gcc")

		set(CPPCHECK_OPTIONS
			${CPPCHECK}
			--template=${CPPCHECK_TEMPLATE}
			--enable=style,performance,warning,portability
			--inline-suppr
			--suppress=cppcheckError
			--suppress=internalAstError
			--suppress=unmatchedSuppression
			--suppress=passedByValue
			--suppress=syntaxError
			--suppress=preprocessorErrorDirective
			--inconclusive
		)

		if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
			set(CPPCHECK_OPTIONS ${CPPCHECK_OPTIONS} --std=c++${CMAKE_CXX_STANDARD})
		endif()

		if(ee_WARNINGS_AS_ERRORS)
			list(APPEND CPPCHECK_OPTIONS --error-exitcode=2)
		endif()

		set_target_properties(
			ee_project_options PROPERTIES
			CXX_CPPCHECK ${CPPCHECK_OPTIONS}
		)

		message(${WARNING_MESSAGE} "ccpcheck set up")
	else()
		message(${WARNING_MESSAGE} "cppcheck requested but executable not found")
	endif()

endmacro()
