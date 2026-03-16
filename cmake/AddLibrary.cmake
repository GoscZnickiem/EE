function(ee_add_library LIB_NAME)

	file(GLOB_RECURSE LIB_INCLUDES CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
	)

	if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
		file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
		)
		set(IS_HEADER_ONLY FALSE)
		add_library(${LIB_NAME} ${LIB_SOURCES} ${LIB_INCLUDES})
		target_link_libraries(${LIB_NAME}
			PRIVATE
			ee_project_options
			${ARGN}
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
			${ARGN}
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
