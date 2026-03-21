file(GLOB EE_LIBRARIES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

foreach(child ${EE_LIBRARIES})
	if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${child}" AND
			EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt")
		add_subdirectory(${child})
	endif()
endforeach()
