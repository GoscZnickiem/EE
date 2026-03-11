include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(ee_sanitizers_support)
    if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
        set(SUPPORTS_UBSAN ON)
    else()
        set(SUPPORTS_UBSAN OFF)
    endif()
    if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
        set(SUPPORTS_ASAN OFF)
    else()
        set(SUPPORTS_ASAN ON)
    endif()
endmacro()

macro(ee_options_setup)
	option(ee_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
	option(ee_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
	option(ee_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
	option(ee_ENABLE_PCH "Enable precompiled headers" OFF)
    #TODO:
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
		ee_ENABLE_PCH
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

	if(ee_ENABLE_PCH)
		target_precompile_headers(
			ee_options
			INTERFACE
			<utility>
			<fstream>
			<algorithm>
			<filesystem>
			<thread>
			<chrono>
			<functional>
			<memory>
			<cmath>
			<numeric>
			<vector>
			<array>
			<string>
			<map>
			<unordered_map>
			<initializer_list>
			<queue>
			<list>
			<cstdint>
			<stdexcept>)
	endif()

	if(${ee_BUILD_TESTS})
		message("###################################################################")
		message("#                         Building tests                          #")
		message("###################################################################")
		find_package(Catch2 3 REQUIRED)
	endif()

endmacro()
