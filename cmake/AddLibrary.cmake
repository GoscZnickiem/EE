function(ee_add_library LIB_NAME)

	cmake_parse_arguments(
		EE_LIB
		""                             # no options
		""                             # no single-value args
		"PUBLIC;PRIVATE;INTERFACE"     # multi-value args
		${ARGN}
	)

	if(NOT EE_LIB_PUBLIC AND NOT EE_LIB_PRIVATE AND NOT EE_LIB_INTERFACE)
		set(EE_LIB_PRIVATE ${ARGN})
	endif()

	file(GLOB_RECURSE LIB_INCLUDES CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
	)

	if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
		file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
		)

		add_library(${LIB_NAME} ${LIB_SOURCES} ${LIB_INCLUDES})

		target_link_libraries(${LIB_NAME}
			PRIVATE ee_project_options
			PRIVATE ${EE_LIB_PRIVATE}
			PUBLIC  ${EE_LIB_PUBLIC}
			INTERFACE ${EE_LIB_INTERFACE}
		)

		target_include_directories(${LIB_NAME}
			PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/include
		)
	else()
		add_library(${LIB_NAME} INTERFACE)

		target_link_libraries(${LIB_NAME}
			INTERFACE
			ee_project_options
			${EE_LIB_PRIVATE}
			${EE_LIB_PUBLIC}
			${EE_LIB_INTERFACE}
		)

		target_include_directories(${LIB_NAME}
			INTERFACE
			${CMAKE_CURRENT_SOURCE_DIR}/include
		)
	endif()


	if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")
		ee_add_test_subdirectory(test)
	endif()

endfunction()
