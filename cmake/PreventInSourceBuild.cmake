function(ee_prevent_out_of_source_builds)
	get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
	get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

	if("${srcdir}" STREQUAL "${bindir}")
		message(FATAL_ERROR "Forbidden build target directory - cannot build in root")
	endif()
endfunction()

ee_prevent_out_of_source_builds()
