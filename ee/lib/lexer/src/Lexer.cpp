#include "lexer/Lexer.hpp"

#include "lexer/Token.hpp"

namespace ee::lex {

Lexer::emitted_type Lexer::emit() {
	return Token{};
}

bool Lexer::done() const {
	return input_empty();
}

} // namespace ee::lex
