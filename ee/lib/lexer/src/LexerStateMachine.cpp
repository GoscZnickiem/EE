#include "LexerStateMachine.hpp"

#include "lexer/Exception.hpp"

#include <cctype>
#include <optional>
#include <stdexcept>
#include <string>
#include <types/Token.hpp>
#include <types/Types.hpp>

namespace ee::lex {

namespace {

// TODO: move it
std::string to_hex(unsigned char c) {
	const char* hex = "0123456789ABCDEF";
	std::string result;
	constexpr auto end = 0xfu;
	result += hex[(static_cast<unsigned int>(c) >> 4u) & end];
	result += hex[c & end];
	return result;
}

// TODO: move it
std::string pretty_char(char c) {
	auto uc = static_cast<unsigned char>(c);

	switch (c) {
		case '\n': return R"('\n')";
		case '\t': return R"('\t')";
		case '\r': return R"('\r')";
		case '\0': return R"('\0')";
		case '\v': return R"('\v')";
		case '\f': return R"('\f')";
		case '\\': return R"('\\')";
		case '\'': return R"('\'')";
		default:
			if (std::isprint(uc) != 0) {
				return std::string("'") + c + "'";
			} else {
				return "'\\x" + to_hex(uc) + "'";
			}
	}
}

bool is_space(Char c) {
	return std::isspace(c) != 0;
}

bool is_name_start(Char c) {
	return std::isalpha(c) != 0 || c == '_';
}

bool is_name_char(Char c) {
	return std::isalnum(c) != 0 || c == '_';
}

bool is_digit(Char c) {
	return std::isdigit(c) != 0;
}

bool is_bin_digit(Char c) {
	return c == '0' || c == '1';
}

bool is_oct_digit(Char c) {
	return std::isdigit(c) != 0 && c != '9';
}

bool is_hex_digit(Char c) {
	return std::isxdigit(c) != 0;
}

bool is_symbol(Char c) {
	return std::ispunct(c) != 0 && c != '_' && c != '{' && c != '}';
}

} // namespace

StateMachine::Result StateMachine::step(Char c) {
	switch (state) {
		case State::INIT				 : return state_init(c);
		case State::SLASH				 : return state_slash(c);
		case State::DOT					 : return state_dot(c);
		case State::ZERO				 : return state_zero(c);
		case State::ZERO_B				 : return state_zero_b(c);
		case State::ZERO_O				 : return state_zero_o(c);
		case State::ZERO_X				 : return state_zero_x(c);
		case State::COMMENT_LINE		 : return state_comment_line(c);
		case State::COMMENT_BLOCK		 : return state_comment_block(c);
		case State::COMMENT_STAR		 : return state_comment_star(c);
		case State::STRING_LITERAL		 : return state_string_literal(c);
		case State::STRING_LITERAL_ESC	 : return state_string_literal_esc(c);
		case State::RAW_STRING_LITERAL	 : return state_raw_string_literal(c);
		case State::NUMERIC_LITERAL		 : return state_numeric_literal(c);
		case State::NUMERIC_BIN_LITERAL	 : return state_numeric_bin_literal(c);
		case State::NUMERIC_OCT_LITERAL	 : return state_numeric_oct_literal(c);
		case State::NUMERIC_HEX_LITERAL	 : return state_numeric_hex_literal(c);
		case State::FLOAT_LITERAL		 : return state_float_literal(c);
		case State::FLOAT_E				 : return state_float_e(c);
		case State::FLOAT_E_LITERAL		 : return state_float_e_literal(c);
		case State::NUMERIC_LITERAL_SUFIX: return state_numeric_literal_sufix(c);
		case State::CHAR_LITERAL		 : return state_char_literal(c);
		case State::NAME				 : return state_name(c);
		case State::SYMBOL				 : return state_symbol(c);
	}
}

std::optional<Token::Type> StateMachine::pull_last() const {
	switch (state) {
		case State::INIT				 : [[fallthrough]];
		case State::COMMENT_LINE		 : [[fallthrough]];
		case State::COMMENT_BLOCK		 : [[fallthrough]];
		case State::COMMENT_STAR		 : return std::nullopt;
		case State::STRING_LITERAL		 : [[fallthrough]];
		case State::STRING_LITERAL_ESC	 : [[fallthrough]];
		case State::RAW_STRING_LITERAL	 : throw err::UnterminatedStringError("Missing termination character '\"'.");
		case State::CHAR_LITERAL		 : throw err::UnterminatedCharacterLiteralError("Missing termination character \"'\".");
		case State::ZERO				 : [[fallthrough]];
		case State::NUMERIC_LITERAL		 : [[fallthrough]];
		case State::NUMERIC_BIN_LITERAL	 : [[fallthrough]];
		case State::NUMERIC_OCT_LITERAL	 : [[fallthrough]];
		case State::NUMERIC_HEX_LITERAL	 : [[fallthrough]];
		case State::FLOAT_LITERAL		 : [[fallthrough]];
		case State::FLOAT_E				 : [[fallthrough]];
		case State::NUMERIC_LITERAL_SUFIX: [[fallthrough]];
		case State::FLOAT_E_LITERAL		 : return Token::Type::NUMERIC_LITERAL;
		case State::ZERO_B				 : [[fallthrough]];
		case State::ZERO_O				 : [[fallthrough]];
		case State::ZERO_X				 : throw err::IncompleteTokenError("Incomplete numeric literal.");
		case State::NAME				 : return Token::Type::NAME;
		case State::SLASH				 : [[fallthrough]];
		case State::DOT					 : [[fallthrough]];
		case State::SYMBOL				 : return Token::Type::SYMBOL;
	}
}

StateMachine::Result StateMachine::ret(State s, CharResult c) {
	state = s;
	return {c, std::nullopt};
}

StateMachine::Result StateMachine::ret(Token::Type t, CharResult c) {
	state = State::INIT;
	return {c, t};
}

StateMachine::Result StateMachine::state_init(Char c) {
	if (is_space(c)) return ret(State::INIT, CharResult::DISCARD);
	if (c == ';') return ret(Token::Type::SEMICOLON, CharResult::ACCEPT);
	if (c == ',') return ret(Token::Type::COMMA, CharResult::ACCEPT);
	if (c == '{') return ret(Token::Type::BRACE_L, CharResult::ACCEPT);
	if (c == '}') return ret(Token::Type::BRACE_R, CharResult::ACCEPT);
	if (c == '/') return ret(State::SLASH, CharResult::ACCEPT);
	if (c == '.') return ret(State::DOT, CharResult::ACCEPT);
	if (c == '0') return ret(State::ZERO, CharResult::ACCEPT);
	if (c == '"') return ret(State::STRING_LITERAL, CharResult::ACCEPT);
	if (c == '\'') return ret(State::CHAR_LITERAL, CharResult::ACCEPT);
	if (is_name_start(c)) return ret(State::NAME, CharResult::ACCEPT);
	if (is_digit(c)) return ret(State::NUMERIC_LITERAL, CharResult::ACCEPT);
	if (is_symbol(c)) return ret(State::SYMBOL, CharResult::ACCEPT);

	throw std::runtime_error("what the fuck happened? What is this: " + pretty_char(c) + ".");
}

StateMachine::Result StateMachine::state_slash(Char c) {
	if (c == '/') return ret(State::COMMENT_LINE, CharResult::CLEAR);
	if (c == '*') return ret(State::COMMENT_BLOCK, CharResult::CLEAR);
	return ret(State::SYMBOL, CharResult::ACCEPT);
}

StateMachine::Result StateMachine::state_dot(Char c) {
	if (is_digit(c)) return ret(State::FLOAT_LITERAL, CharResult::ACCEPT);
	if (is_symbol(c)) return ret(State::SYMBOL, CharResult::ACCEPT);
	return ret(Token::Type::SYMBOL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_zero(Char c) {
	if (is_digit(c)) return ret(State::NUMERIC_LITERAL, CharResult::ACCEPT);
	if (c == 'b' || c == 'B') return ret(State::ZERO_B, CharResult::ACCEPT);
	if (c == 'o' || c == 'O') return ret(State::ZERO_O, CharResult::ACCEPT);
	if (c == 'x' || c == 'X') return ret(State::ZERO_X, CharResult::ACCEPT);
	if (c == 'i' || c == 'I' || c == 'u' || c == 'U' || c == 'f' || c == 'F') return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	if (c == '.') return ret(State::FLOAT_LITERAL, CharResult::ACCEPT);
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_zero_b(Char c) {
	if (is_bin_digit(c)) return ret(State::NUMERIC_BIN_LITERAL, CharResult::ACCEPT);
	if (is_digit(c)) throw err::InvalidDigitError("Invalid digit " + pretty_char(c) + " in binary numeric literal.");
	if (is_name_start(c)) throw err::IncompleteTokenError("Binary numeric literal expected but received " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_zero_o(Char c) {
	if (is_oct_digit(c)) return ret(State::NUMERIC_OCT_LITERAL, CharResult::ACCEPT);
	if (is_digit(c)) throw err::InvalidDigitError("Invalid digit " + pretty_char(c) + " in octal numeric literal.");
	if (is_name_start(c)) throw err::IncompleteTokenError("Octal numeric literal expected but received " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_zero_x(Char c) {
	if (is_hex_digit(c)) return ret(State::NUMERIC_HEX_LITERAL, CharResult::ACCEPT);
	if (is_name_start(c)) throw err::IncompleteTokenError("Hexadecimal numeric literal expected but received " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_comment_line(Char c) {
	return ret(c == '\n' ? State::INIT : State::COMMENT_LINE, CharResult::DISCARD);
}

StateMachine::Result StateMachine::state_comment_block(Char c) {
	return ret(c == '*' ? State::COMMENT_STAR : State::COMMENT_BLOCK, CharResult::DISCARD);
}

StateMachine::Result StateMachine::state_comment_star(Char c) {
	return ret(c == '/' ? State::INIT : State::COMMENT_BLOCK, CharResult::DISCARD);
}

StateMachine::Result StateMachine::state_string_literal(Char c) {
	if (c == '"') return ret(Token::Type::STRING_LITERAL, CharResult::ACCEPT);
	if (c == '\\') return ret(State::STRING_LITERAL_ESC, CharResult::ACCEPT);
	if (c == '\n') throw std::runtime_error("Newline character inside string literal!");
	return ret(State::STRING_LITERAL, CharResult::ACCEPT);
}

StateMachine::Result StateMachine::state_string_literal_esc(Char c) {
	if (c == '\n') throw std::runtime_error("Newline character inside string literal!");
	return ret(State::STRING_LITERAL, CharResult::ACCEPT);
}

StateMachine::Result StateMachine::state_raw_string_literal(Char c) {
	if (c == '"') return ret(Token::Type::STRING_LITERAL, CharResult::ACCEPT);
	if (c == '\n') throw std::runtime_error("Newline character inside string literal!");
	return ret(State::RAW_STRING_LITERAL, CharResult::ACCEPT);
}

StateMachine::Result StateMachine::state_char_literal(Char c) {
	throw std::runtime_error("not implemented " + pretty_char(c));
}

StateMachine::Result StateMachine::state_numeric_literal(Char c) {
	if (is_digit(c)) return ret(State::NUMERIC_LITERAL, CharResult::ACCEPT);
	if (c == 'i' || c == 'I' || c == 'u' || c == 'U' || c == 'f' || c == 'F') return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	if (c == 'e' || c == 'E') return ret(State::FLOAT_E, CharResult::ACCEPT);
	if (c == '.') return ret(State::FLOAT_LITERAL, CharResult::ACCEPT);
	if (is_name_char(c)) throw err::ExpectedNumericLiteralSufix("Invalid numeric literal sufix " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_numeric_bin_literal(Char c) {
	if (is_bin_digit(c)) return ret(State::NUMERIC_BIN_LITERAL, CharResult::ACCEPT);
	if (c == 'i' || c == 'I' || c == 'u' || c == 'U') return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	if (is_digit(c)) throw err::InvalidDigitError("Invalid digit " + pretty_char(c) + " in binary numeric literal.");
	if (is_name_start(c)) throw err::ExpectedNumericLiteralSufix("Invalid numeric literal sufix " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_numeric_oct_literal(Char c) {
	if (is_oct_digit(c)) return ret(State::NUMERIC_OCT_LITERAL, CharResult::ACCEPT);
	if (c == 'i' || c == 'I' || c == 'u' || c == 'U') return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	if (is_digit(c)) throw err::InvalidDigitError("Invalid digit " + pretty_char(c) + " in octal numeric literal.");
	if (is_name_start(c)) throw err::ExpectedNumericLiteralSufix("Invalid numeric literal sufix " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_numeric_hex_literal(Char c) {
	if (is_hex_digit(c)) return ret(State::NUMERIC_HEX_LITERAL, CharResult::ACCEPT);
	if (c == 'i' || c == 'I' || c == 'u' || c == 'U') return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	if (is_name_start(c)) throw err::ExpectedNumericLiteralSufix("Invalid numeric literal sufix " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_float_literal(Char c) {
	if (is_digit(c)) return ret(State::FLOAT_LITERAL, CharResult::ACCEPT);
	if (c == 'f' || c == 'F') return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	if (c == 'e' || c == 'E') return ret(State::FLOAT_E, CharResult::ACCEPT);
	if (is_name_start(c)) throw err::ExpectedNumericLiteralSufix("Invalid numeric literal sufix " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_float_e(Char c) {
	if (is_digit(c) || c == '-' || c == '+') return ret(State::FLOAT_E_LITERAL, CharResult::ACCEPT);
	throw err::ExpectedExponentialFloatNotation("Expected exponential float notation.");
}

StateMachine::Result StateMachine::state_float_e_literal(Char c) {
	if (is_digit(c)) return ret(State::FLOAT_E_LITERAL, CharResult::ACCEPT);
	if (c == 'f' || c == 'F') return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	if (is_name_char(c)) throw err::ExpectedNumericLiteralSufix("Invalid numeric literal sufix " + pretty_char(c) + ".");
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_numeric_literal_sufix(Char c) {
	if (is_digit(c) || is_name_char(c)) return ret(State::NUMERIC_LITERAL_SUFIX, CharResult::ACCEPT);
	return ret(Token::Type::NUMERIC_LITERAL, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_name(Char c) {
	if (is_name_char(c)) return ret(State::NAME, CharResult::ACCEPT);
	if (c == '"' && (*buffer == "w" || *buffer == "u8" || *buffer == "u16" || *buffer == "u32"))
		return ret(State::STRING_LITERAL, CharResult::ACCEPT);
	if (c == '"' && (*buffer == "r" || *buffer == "rw" || *buffer == "ru8" || *buffer == "ru16" || *buffer == "ru32"))
		return ret(State::RAW_STRING_LITERAL, CharResult::ACCEPT);
	return ret(Token::Type::NAME, CharResult::LEAVE);
}

StateMachine::Result StateMachine::state_symbol(Char c) {
	throw std::runtime_error("not implemented " + pretty_char(c));
}

} // namespace ee::lex
