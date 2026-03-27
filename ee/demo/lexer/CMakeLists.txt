add_executable(demo_lexer)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

target_sources(demo_lexer
	PRIVATE ${SOURCES}
)

target_link_libraries(demo_lexer
	PRIVATE ee_project_options lexer
)
