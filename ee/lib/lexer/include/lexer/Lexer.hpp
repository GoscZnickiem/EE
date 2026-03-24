#ifndef EE_LIB_LEXER_LEXER
#define EE_LIB_LEXER_LEXER

#include <compiler_unit/Unit.hpp>
#include <optional>
#include <types/Character.hpp>
#include <types/Token.hpp>

namespace ee::lex {

class Lexer : public cmpu::Unit<Character, Token> {
private:
	enum class State {
		INIT,
		SLASH,
		DOT,
		COMMENT_LINE,
		COMMENT_BLOCK,
		STRING_LITERAL,
		NUMERIC_LITERAL,
		CHAR_LITERAL,
		IDENTIFIER,
		SYMBOL,
	};

	bool state_init();
	bool state_slash();
	bool state_dot();
	bool state_comment_line();
	bool state_comment_block();
	bool state_string_literal();
	bool state_numeric_literal();
	bool state_char_literal();
	bool state_identifier();
	bool state_symbol();

	State state{State::INIT};
	std::optional<Character> character;
	Char c{};
	Token buffer{};

	bool push();
	bool push(State s);
	bool push(Token::Type t);
	bool step();

public:
	emitted_type emit() override;

	[[nodiscard]] bool done() override;
};

} // namespace ee::lex

#endif // !EE_LIB_LEXER_LEXER
