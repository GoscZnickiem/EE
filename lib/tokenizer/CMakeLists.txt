file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(ee_tokenizer ${SOURCES})

target_include_directories(ee_tokenizer
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ee_tokenizer
	PRIVATE
	ee_project_options
)

add_test_subdirectory(test)
