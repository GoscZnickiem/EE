file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(tokenizer ${SOURCES})

target_include_directories(tokenizer
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(test)
