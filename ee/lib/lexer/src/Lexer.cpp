#include "lexer/Lexer.hpp"

namespace ee::lex {

Lexer::emitted_type Lexer::emit() {

	switch (state) {
		case State::INIT:
		case State::COMMENT_LINE:
		case State::COMMENT_BLOCK:
		case State::STRING_LITERAL:
		case State::NUMERIC_LITERAL:
		case State::CHAR_LITERAL:
		case State::IDENTIFIER:
		case State::SYMBOL:
		case State::DOT:
		case State::SLASH		   : break;
	}

	return Token{};
}

bool Lexer::done() {
	return input_empty();
}

} // namespace ee::lex
