#include "lexer/Lexer.hpp"
#include <cassert>

namespace ee::lex {

void Lexer::step() {
	if (!character) {
		if (input_empty()) {
			buffer.type = Token::Type::INVALID;
			reading = false;
			return;
		}
		character = get();
	}
	c = **character;

	switch (state) {
		case State::INIT		   : state_init(); break;
		case State::COMMENT_LINE   : state_comment_line(); break;
		case State::COMMENT_BLOCK  : state_comment_block(); break;
		case State::COMMENT_STAR   : state_comment_star(); break;
		case State::STRING_LITERAL : state_string_literal(); break;
		case State::NUMERIC_LITERAL: state_numeric_literal(); break;
		case State::CHAR_LITERAL   : state_char_literal(); break;
		case State::IDENTIFIER	   : state_identifier(); break;
		case State::SYMBOL		   : state_symbol(); break;
		case State::DOT			   : state_dot(); break;
		case State::SLASH		   : state_slash(); break;
	}
}

void Lexer::push() {
	assert(character);

	if (buffer.data.empty()) {
		buffer.meta_data.lineStart = character->meta_data.line;
		buffer.meta_data.columnStart = character->meta_data.column;
	}

	buffer.meta_data.lineEnd = character->meta_data.line;
	buffer.meta_data.columnEnd = character->meta_data.column;

	buffer.data += c;
	character.reset();
}

void Lexer::push(Token::Type t) {
	push();
	buffer.type = t;
	reading = false;
}

void Lexer::push(State s) {
	push();
	state = s;
}

void Lexer::discard() {
	character.reset();
}

void Lexer::discard(State s) {
	state = s;
	character.reset();
}

void Lexer::read_next_token() {
	buffer.data.clear();
	buffer.type = Token::Type::INVALID;
	state = State::INIT;
	reading = true;
	while (reading) step();
}

Lexer::emitted_type Lexer::emit() {
	if (current_token.type == Token::Type::INVALID) read_next_token();
	current_token = buffer;
	read_next_token();
	return current_token;
}

bool Lexer::done() {
	return buffer.type == Token::Type::INVALID && current_token.type != Token::Type::INVALID;
}

} // namespace ee::lex
