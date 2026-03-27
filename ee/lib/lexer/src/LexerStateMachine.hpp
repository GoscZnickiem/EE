#ifndef EE_LIB_LEXER_LEXERSTATEMACHINE
#define EE_LIB_LEXER_LEXERSTATEMACHINE

#include "types/Types.hpp"

#include <compiler_unit/Unit.hpp>
#include <optional>
#include <types/Character.hpp>
#include <types/Token.hpp>
#include <utility>

namespace ee::lex {

class StateMachine {
public:
	explicit StateMachine(String& buffered_word) : buffer(&buffered_word) {}

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
		DOT,
		ZERO,
		ZERO_B,
		ZERO_O,
		ZERO_X,

		COMMENT_LINE,
		COMMENT_BLOCK,
		COMMENT_STAR,

		STRING_LITERAL,
		STRING_LITERAL_ESC,
		RAW_STRING_LITERAL,
		CHAR_LITERAL,
		CHAR_LITERAL_ESC,

		NUMERIC_LITERAL,
		NUMERIC_BIN_LITERAL,
		NUMERIC_OCT_LITERAL,
		NUMERIC_HEX_LITERAL,
		FLOAT_LITERAL,
		FLOAT_E,
		FLOAT_E_LITERAL,
		NUMERIC_LITERAL_SUFIX,

		NAME,
		SYMBOL,
	};

	[[nodiscard]] Result state_init(Char c);
	[[nodiscard]] Result state_slash(Char c);
	[[nodiscard]] Result state_dot(Char c);
	[[nodiscard]] Result state_zero(Char c);
	[[nodiscard]] Result state_zero_b(Char c);
	[[nodiscard]] Result state_zero_o(Char c);
	[[nodiscard]] Result state_zero_x(Char c);

	[[nodiscard]] Result state_comment_line(Char c);
	[[nodiscard]] Result state_comment_block(Char c);
	[[nodiscard]] Result state_comment_star(Char c);

	[[nodiscard]] Result state_string_literal(Char c);
	[[nodiscard]] Result state_string_literal_esc(Char c);
	[[nodiscard]] Result state_raw_string_literal(Char c);
	[[nodiscard]] Result state_char_literal(Char c);
	[[nodiscard]] Result state_char_literal_esc(Char c);

	[[nodiscard]] Result state_numeric_literal(Char c);
	[[nodiscard]] Result state_numeric_bin_literal(Char c);
	[[nodiscard]] Result state_numeric_oct_literal(Char c);
	[[nodiscard]] Result state_numeric_hex_literal(Char c);
	[[nodiscard]] Result state_float_literal(Char c);
	[[nodiscard]] Result state_float_e(Char c);
	[[nodiscard]] Result state_float_e_literal(Char c);
	[[nodiscard]] Result state_numeric_literal_sufix(Char c);

	[[nodiscard]] Result state_name(Char c);
	[[nodiscard]] Result state_symbol(Char c);

	[[nodiscard]] Result ret(State s, CharResult c);
	[[nodiscard]] Result ret(Token::Type t, CharResult c);

	const String* buffer;
	State state{State::INIT};
};

} // namespace ee::lex

#endif // !EE_LIB_LEXER_LEXERSTATEMACHINE
