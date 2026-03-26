#include "LexerStateMachine.hpp"

#include "lexer/Exception.hpp"

#include <cctype>
#include <optional>
#include <stdexcept>
#include <string>
#include <types/Token.hpp>
#include <types/Types.hpp>

namespace ee::lex {

StateMachine::Result StateMachine::step(Char c) {
	switch (state) {
		case State::INIT			  : return state_init(c);
		case State::COMMENT_LINE	  : return state_comment_line(c);
		case State::COMMENT_BLOCK	  : return state_comment_block(c);
		case State::COMMENT_STAR	  : return state_comment_star(c);
		case State::RAW_STRING_LITERAL: return state_raw_string_literal(c);
		case State::STRING_LITERAL	  : return state_string_literal(c);
		case State::NUMERIC_LITERAL	  : return state_numeric_literal(c);
		case State::CHAR_LITERAL	  : return state_char_literal(c);
		case State::NAME			  : return state_name(c);
		case State::SYMBOL			  : return state_symbol(c);
		case State::DOT				  : return state_dot(c);
		case State::SLASH			  : return state_slash(c);
	}
}

std::optional<Token::Type> StateMachine::pull_last() const {
	switch (state) {
		case State::INIT			  : [[fallthrough]];
		case State::COMMENT_LINE	  : [[fallthrough]];
		case State::COMMENT_BLOCK	  : [[fallthrough]];
		case State::COMMENT_STAR	  : return std::nullopt;
		case State::STRING_LITERAL	  : [[fallthrough]];
		case State::RAW_STRING_LITERAL: throw err::UnterminatedStringError("Missing termination character '\"'.");
		case State::NUMERIC_LITERAL	  : return Token::Type::NUMERIC_LITERAL;
		case State::CHAR_LITERAL	  : throw err::UnterminatedCharacterLiteralError("Missing termination character \"'\".");
		case State::NAME			  : return Token::Type::NAME;
		case State::SYMBOL			  : [[fallthrough]];
		case State::DOT				  : [[fallthrough]];
		case State::SLASH			  : return Token::Type::SYMBOL;
	}
}

StateMachine::Result StateMachine::ret(State s, CharResult c) {
	state = s;
	return {c, std::nullopt};
}

StateMachine::Result StateMachine::ret(State s, CharResult c, Token::Type t) {
	state = s;
	return {c, t};
}

StateMachine::Result StateMachine::state_init(Char c) {
	if (std::isspace(c) != 0) return ret(State::INIT, CharResult::DISCARD);
	if (c == ';') return ret(State::INIT, CharResult::ACCEPT, Token::Type::SEMICOLON);
	if (c == ',') return ret(State::INIT, CharResult::ACCEPT, Token::Type::COMMA);
	if (c == '{') return ret(State::INIT, CharResult::ACCEPT, Token::Type::BRACE_L);
	if (c == '}') return ret(State::INIT, CharResult::ACCEPT, Token::Type::BRACE_R);
	if (c == '/') return ret(State::SLASH, CharResult::ACCEPT);
	if (c == '"') return ret(State::STRING_LITERAL, CharResult::ACCEPT);
	if (c == '.') return ret(State::DOT, CharResult::ACCEPT);
	if (c == '\'') return ret(State::CHAR_LITERAL, CharResult::ACCEPT);
	if (std::isalpha(c) != 0 || c == '_') return ret(State::NAME, CharResult::ACCEPT);
	if (std::isdigit(c) != 0) return ret(State::NUMERIC_LITERAL, CharResult::ACCEPT);
	if (std::ispunct(c) != 0) return ret(State::SYMBOL, CharResult::ACCEPT);

	throw std::runtime_error("what the fuck happened? What is this: '" + std::to_string(c) + "'.");
}

StateMachine::Result StateMachine::state_slash(Char c) {
	if (c == '/') return ret(State::COMMENT_LINE, CharResult::CLEAR);
	if (c == '*') return ret(State::COMMENT_BLOCK, CharResult::CLEAR);
	return ret(State::SYMBOL, CharResult::ACCEPT);
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
	if (c == '"') return ret(State::INIT, CharResult::ACCEPT, Token::Type::STRING_LITERAL);
	if (c == '\n') throw std::runtime_error("Newline character inside string literal!");
	throw std::runtime_error("not implemented " + std::to_string(c));
}

StateMachine::Result StateMachine::state_raw_string_literal(Char c) {
	if (c == '"') return ret(State::INIT, CharResult::ACCEPT, Token::Type::STRING_LITERAL);
	if (c == '\n') throw std::runtime_error("Newline character inside string literal!");
	return ret(State::RAW_STRING_LITERAL, CharResult::ACCEPT);
}

StateMachine::Result StateMachine::state_dot(Char c) {
	throw std::runtime_error("not implemented " + std::to_string(c));
}

StateMachine::Result StateMachine::state_numeric_literal(Char c) {
	throw std::runtime_error("not implemented " + std::to_string(c));
}

StateMachine::Result StateMachine::state_char_literal(Char c) {
	throw std::runtime_error("not implemented " + std::to_string(c));
}

StateMachine::Result StateMachine::state_name(Char c) {
	if (std::isalnum(c) != 0 || c == '_') return ret(State::NAME, CharResult::ACCEPT);
	if (c == '"' && (*buffer == "w" || *buffer == "u8" || *buffer == "u16" || *buffer == "u32"))
		return ret(State::STRING_LITERAL, CharResult::ACCEPT);
	if (c == '"' && (*buffer == "r" || *buffer == "rw" || *buffer == "ru8" || *buffer == "ru16" || *buffer == "ru32"))
		return ret(State::RAW_STRING_LITERAL, CharResult::ACCEPT);
	return ret(State::INIT, CharResult::LEAVE, Token::Type::NAME);
}

StateMachine::Result StateMachine::state_symbol(Char c) {
	throw std::runtime_error("not implemented " + std::to_string(c));
}

} // namespace ee::lex
