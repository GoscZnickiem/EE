#ifndef EE_LIB_LEXER_LEXER
#define EE_LIB_LEXER_LEXER

#include <compiler_unit/Unit.hpp>
#include <optional>
#include <types/Character.hpp>
#include <types/Token.hpp>
#include <utility>

namespace ee::lex {

class Lexer : public cmpu::Unit<Character, Token> {
public:
	emitted_type emit() override;

	[[nodiscard]] bool done() override;

private:
	class StateMachine {
	public:
		enum class CharResult {
			ACCEPT,
			DISCARD,
			LEAVE,
			CLEAR
		};

		using Result = std::pair<CharResult, std::optional<Token::Type>>;

		[[nodiscard]] Result step(Char c);
		[[nodiscard]] std::optional<Token::Type> pull_last() const;

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

		[[nodiscard]] Result state_init(Char c);
		[[nodiscard]] Result state_slash(Char c);
		[[nodiscard]] Result state_comment_line(Char c);
		[[nodiscard]] Result state_comment_block(Char c);
		[[nodiscard]] Result state_comment_star(Char c);
		[[nodiscard]] Result state_dot(Char c);
		[[nodiscard]] Result state_string_literal(Char c);
		[[nodiscard]] Result state_numeric_literal(Char c);
		[[nodiscard]] Result state_char_literal(Char c);
		[[nodiscard]] Result state_identifier(Char c);
		[[nodiscard]] Result state_symbol(Char c);

		[[nodiscard]] Result ret(State s, CharResult c);
		[[nodiscard]] Result ret(State s, CharResult c, Token::Type t);

		State state{State::INIT};
	};

	enum class State {
		INIT,
		READY,
		LAST,
		DONE,
	};

	State state{State::INIT};

	std::optional<Character> character;
	Token buffer{.type = Token::Type::INVALID, .data{}, .meta_data{}};

	Token read_token();
};

} // namespace ee::lex

#endif // !EE_LIB_LEXER_LEXER
