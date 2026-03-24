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
		COMMENT_LINE,
		COMMENT_BLOCK,
		COMMENT_STAR,
		DOT,
		STRING_LITERAL,
		NUMERIC_LITERAL,
		CHAR_LITERAL,
		IDENTIFIER,
		SYMBOL,
	};

	void state_init();
	void state_slash();
	void state_comment_line();
	void state_comment_block();
	void state_comment_star();
	void state_dot();
	void state_string_literal();
	void state_numeric_literal();
	void state_char_literal();
	void state_identifier();
	void state_symbol();

	bool reading{false};
	State state{State::INIT};
	std::optional<Character> character;
	Char c{};
	Token buffer{.type = Token::Type::INVALID, .data{}, .meta_data{}};
	Token current_token{.type = Token::Type::INVALID, .data{}, .meta_data{}};

	void push();
	void push(State s);
	void push(Token::Type t);
	void discard();
	void discard(State s);
	void step();
	void read_next_token();

public:
	emitted_type emit() override;

	[[nodiscard]] bool done() override;
};

} // namespace ee::lex

#endif // !EE_LIB_LEXER_LEXER
