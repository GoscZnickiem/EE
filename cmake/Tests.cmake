include(FetchContent)

FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

function(ee_add_ut TARGET)
	add_executable(${TARGET} ${TARGET}.cpp)

	target_link_libraries(${TARGET}
		PUBLIC
		gtest_main
		${ARGN}
	)

	gtest_discover_tests(${TARGET})
endfunction()
