include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(ee_options_setup)

	option(ee_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
	option(ee_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
	option(ee_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(ee_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(ee_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(ee_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(ee_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(ee_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(ee_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)

    mark_as_advanced(
        ee_WARNINGS_AS_ERRORS
        ee_ENABLE_CLANG_TIDY
        ee_ENABLE_CPPCHECK
        ee_ENABLE_IPO
        ee_ENABLE_SANITIZER_ADDRESS
        ee_ENABLE_SANITIZER_LEAK
        ee_ENABLE_SANITIZER_UNDEFINED
        ee_ENABLE_SANITIZER_THREAD
        ee_ENABLE_SANITIZER_MEMORY
    )

endmacro()

macro(ee_apply_options)

	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

	add_library(ee_warnings INTERFACE)
	add_library(ee_options INTERFACE)

	if(NOT ${ee_WARNINGS_AS_ERRORS})
		message("###################################################################")
		message("#   Warning: compiler warnings not treated as errors              #")
		message("#   It is recommended to set this option to ON                    #")
		message("###################################################################")
	endif()

	include(cmake/CompilerWarnings.cmake)
	set_project_warnings(ee_warnings ${ee_WARNINGS_AS_ERRORS} "" "" "" "")

	include(cmake/ClangTidy.cmake)
    if(ee_ENABLE_CLANG_TIDY)
        ee_enable_clang_tidy(ee_options ${ee_WARNINGS_AS_ERRORS})
    endif()

    include(cmake/CppCheck.cmake)
    if(ee_ENABLE_CPPCHECK)
        ee_enable_cppcheck(${ee_WARNINGS_AS_ERRORS} "")
    endif()

	include(cmake/Sanitizers.cmake)
	ee_enable_sanitizers(
		ee_options
		${ee_ENABLE_SANITIZER_ADDRESS}
		${ee_ENABLE_SANITIZER_LEAK}
		${ee_ENABLE_SANITIZER_UNDEFINED}
		${ee_ENABLE_SANITIZER_THREAD}
		${ee_ENABLE_SANITIZER_MEMORY}
	)

endmacro()
