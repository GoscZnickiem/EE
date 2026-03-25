#ifndef EE_LIB_TYPES_TOKEN
#define EE_LIB_TYPES_TOKEN

#include "types/Types.hpp"

#include <cstddef>

namespace ee {

class Token {
public:
	enum class Type {
		INVALID,
		IDENTIFIER,
		SYMBOL,
		NUMERIC_LITERAL,
		STRING_LITERAL,
		CHAR_LITERAL,
		SEMICOLON,
		COMMA,
		BRACE_L,
		BRACE_R
	};

	struct MetaData {
		std::size_t lineStart;
		std::size_t columnStart;
		std::size_t lineEnd;
		std::size_t columnEnd;
	};

	Type type;
	String data;
	MetaData meta_data;
};

} // namespace ee

#endif // !EE_LIB_TYPES_TOKEN
