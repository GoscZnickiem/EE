#ifndef EE_LIB_TYPES_TOKEN
#define EE_LIB_TYPES_TOKEN

#include "types/Types.hpp"

#include <cstddef>

namespace ee {

class Token {
public:
	enum class Type {
		INVALID,
		NAME,
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
		std::size_t line_start;
		std::size_t column_start;
		std::size_t line_end;
		std::size_t column_end;

		auto operator<=>(const MetaData&) const = default;
	};

	Type type;
	String data;
	MetaData meta_data;
};

} // namespace ee

#endif // !EE_LIB_TYPES_TOKEN
