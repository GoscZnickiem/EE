#include "lexer/Lexer.hpp"

#include <cctype>
#include <stdexcept>

namespace ee::lex {

bool Lexer::step() {
	if (!character) character = get();
	c = **character;

	switch (state) {
		case State::INIT		   : return state_init();
		case State::COMMENT_LINE   : return state_comment_line();
		case State::COMMENT_BLOCK  : return state_comment_block();
		case State::STRING_LITERAL : return state_string_literal();
		case State::NUMERIC_LITERAL: return state_numeric_literal();
		case State::CHAR_LITERAL   : return state_char_literal();
		case State::IDENTIFIER	   : return state_identifier();
		case State::SYMBOL		   : return state_symbol();
		case State::DOT			   : return state_dot();
		case State::SLASH		   : return state_slash();
	}
}

bool Lexer::push() {
	if (buffer.data.empty()) {
		buffer.meta_data.lineStart = character->meta_data.line;
		buffer.meta_data.columnStart = character->meta_data.column;
	}

	buffer.meta_data.lineEnd = character->meta_data.line;
	buffer.meta_data.columnEnd = character->meta_data.column;

	buffer.data += **character;

	return false;
}

bool Lexer::push(Token::Type t) {
	push();
	buffer.type = t;
	return true;
}

bool Lexer::push(State s) {
	state = s;
	return push();
}

bool Lexer::state_init() {
	if (std::isspace(c) != 0) return false;

	if (c == ';') return push(Token::Type::SEMICOLON);
	if (c == ',') return push(Token::Type::COMMA);
	if (c == '{') return push(Token::Type::BRACE_L);
	if (c == '}') return push(Token::Type::BRACE_R);
	if (c == '/') return push(State::SLASH);
	if (c == '"') return push(State::STRING_LITERAL);
	if (c == '\'') return push(State::CHAR_LITERAL);
	if (c == '.') return push(State::DOT);
	if (std::isalpha(c) != 0 || c == '_') return push(State::IDENTIFIER);
	if (std::isdigit(c) != 0) return push(State::NUMERIC_LITERAL);
	if (std::ispunct(c) != 0) return push(State::SYMBOL);

	throw std::runtime_error("what the fuck happened? What is this: '" + std::to_string(c) + "'.");
}

bool Lexer::state_slash() {
	if (c == '/') {
		buffer.data.clear();
		state = State::COMMENT_LINE;
	} else if (c == '*') {
		buffer.data.clear();
		state = State::COMMENT_BLOCK;
	} else {
		state = State::SYMBOL;
	}
	return false;
}

bool Lexer::state_dot() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

bool Lexer::state_comment_line() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

bool Lexer::state_comment_block() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

bool Lexer::state_string_literal() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

bool Lexer::state_numeric_literal() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

bool Lexer::state_char_literal() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

bool Lexer::state_identifier() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

bool Lexer::state_symbol() {
	throw std::runtime_error("not implemented " + std::to_string(**character));
}

Lexer::emitted_type Lexer::emit() {
	buffer.data.clear();

	state = State::INIT;
	while (!step());

	return buffer;
}

bool Lexer::done() {
	return input_empty();
}

} // namespace ee::lex
