#ifndef EE_LIB_LEXER_TOKEN
#define EE_LIB_LEXER_TOKEN

#include <cstddef>
#include <types/Types.hpp>

namespace ee::lex {

class Token {
public:
	enum class Type {
		NAME,
		SYMBOL,
		NUMERAL,
		STRING,
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

} // namespace ee::lex

#endif // !EE_LIB_LEXER_TOKEN
