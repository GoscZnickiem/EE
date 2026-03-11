cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

project(EELang
    VERSION 0.0.0
	DESCRIPTION "The Extra Eccentric language tools"
    # HOMEPAGE_URL ""
    LANGUAGES CXX
)

include(cmake/PreventInSourceBuild.cmake)

include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ProjectOptions.cmake)
include(cmake/ProjectOptions.cmake)
ee_options_setup()
ee_apply_options()
target_compile_features(ee_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_subdirectory(src)
