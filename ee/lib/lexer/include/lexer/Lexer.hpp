#ifndef EE_LIB_LEXER_LEXER
#define EE_LIB_LEXER_LEXER

#include "types/Token.hpp"
#include "types/Character.hpp"

#include <compiler_unit/Unit.hpp>

namespace ee::lex {

class Lexer : public cmpu::Unit<Character, Token> {
private:
	enum class State {
		INIT,
		COMMENT_LINE,
		COMMENT_BLOCK,
		STRING_LITERAL,
		NUMERIC_LITERAL,
		CHAR_LITERAL,
		IDENTIFIER,
		SYMBOL,
		DOT,
		SLASH
	};

	State state{State::INIT};
	String buffer;

public:
	emitted_type emit() override;

	[[nodiscard]] bool done() override;
};

} // namespace ee::lex

#endif // !EE_LIB_LEXER_LEXER
