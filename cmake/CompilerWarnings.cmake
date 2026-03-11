macro(set_project_warnings)

	set(BASE_WARNINGS
		-Wall
		-Wextra
		-Wshadow
		-Wnon-virtual-dtor
		-Wold-style-cast
		-Wcast-align
		-Wunused
		-Woverloaded-virtual
		-Wpedantic
		-Wconversion
		-Wsign-conversion
		-Wnull-dereference
		-Wdouble-promotion
		-Wformat=2
		-Wimplicit-fallthrough
	)

	set(GCC_EXTRA_WARNINGS
		-Wmisleading-indentation
		-Wduplicated-cond
		-Wduplicated-branches
		-Wlogical-op
		-Wuseless-cast
	)

	set(CLANG_EXTRA_WARNINGS

	)

	if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
		set(PROJECT_WARNINGS_CXX ${BASE_WARNINGS} ${CLANG_EXTRA_WARNINGS})
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(PROJECT_WARNINGS_CXX ${BASE_WARNINGS} ${GCC_EXTRA_WARNINGS})
	else()
		message(WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
		set(PROJECT_WARNINGS_CXX "")
	endif()

	if(ee_WARNINGS_AS_ERRORS)
		list(APPEND PROJECT_WARNINGS_CXX -Werror)
	endif()

	add_compile_options(${PROJECT_WARNINGS_CXX})

endmacro()
