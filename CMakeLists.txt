cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(EELang
    VERSION 0.0.0
	DESCRIPTION "The Extra Eccentric Language"
    # HOMEPAGE_URL ""
    LANGUAGES CXX
)

include(cmake/PreventInSourceBuild.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(__BUILD_DEBUG__)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(__BUILD_RELEASE__)
elseif(CMAKE_BUILD_TYPE STREQUAL "Dist")
	add_compile_definitions(__BUILD_DIST__)
endif()

if(${USE_COMPILER} STREQUAL "cl")
	add_compile_definitions(__COMPILER_CL__)
elseif(${USE_COMPILER} STREQUAL "clang-cl")
	add_compile_definitions(__COMPILER_CLANG_CL__)
elseif(${USE_COMPILER} STREQUAL "gcc")
	add_compile_definitions(__COMPILER_GCC__)
elseif(${USE_COMPILER} STREQUAL "clang")
	add_compile_definitions(__COMPILER_CLANG__)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ee_project_options INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings()

if(ee_CLANG_TIDY)
	include(cmake/ClangTidy.cmake)
	ee_enable_clang_tidy()
endif()

if(ee_CPPCHECK)
	include(cmake/CppCheck.cmake)
	ee_enable_cppcheck()
endif()

if(ee_IPO)
	include(cmake/Ipo.cmake)
	ee_ipo()
endif()

if(ee_BUILD_TESTS)
	include(cmake/Tests.cmake)
endif()
include(cmake/AddTestSubdirectory.cmake)

add_subdirectory(ee)
