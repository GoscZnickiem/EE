function(ee_apply_options TARGET)
	foreach(tool IN ITEMS CLANG_TIDY CPPCHECK INCLUDE_WHAT_YOU_USE)
		get_property(_val TARGET ee_project_options PROPERTY EE_${tool})
		if(_val)
			set_target_properties(${TARGET} PROPERTIES
				CXX_${tool} "${_val}"
			)
		endif()
	endforeach()
endfunction()
