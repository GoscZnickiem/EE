#include "lexer/Lexer.hpp"
#include "types/Token.hpp"

#include <cctype>
#include <stdexcept>
#include <string>

namespace ee::lex {

namespace {

int test() {
	return 0;
}

} // namespace

void Lexer::state_init() {
	if (std::isspace(c) != test()) discard();
	else if (c == ';') push(Token::Type::SEMICOLON);
	else if (c == ',') push(Token::Type::COMMA);
	else if (c == '{') push(Token::Type::BRACE_L);
	else if (c == '}') push(Token::Type::BRACE_R);
	else if (c == '.') push(State::DOT);
	else if (c == '/') push(State::SLASH);
	else if (c == '"') push(State::STRING_LITERAL);
	else if (c == '\'') push(State::CHAR_LITERAL);
	else if (std::isalpha(c) != 0 || c == '_') push(State::IDENTIFIER);
	else if (std::isdigit(c) != 0) push(State::NUMERIC_LITERAL);
	else if (std::ispunct(c) != 0) push(State::SYMBOL);
	else throw std::runtime_error("what the fuck happened? What is this: '" + std::to_string(c) + "'.");
}

void Lexer::state_slash() {
	if (c == '/') discard(State::COMMENT_LINE);
	else if (c == '*') discard(State::COMMENT_BLOCK);
	else state = State::SYMBOL;
}

void Lexer::state_comment_line() {
	if (c == '\n') discard(State::INIT);
	discard();
}

void Lexer::state_comment_block() {
	if (c == '*') discard(State::COMMENT_STAR);
	discard();
}

void Lexer::state_comment_star() {
	discard(c == '/' ? State::INIT : State::COMMENT_BLOCK);
}

void Lexer::state_dot() {
	if(c == '0') discard();
	throw std::runtime_error("not implemented " + std::to_string(c));
}

void Lexer::state_string_literal() {
	if(c == '0') discard();
	throw std::runtime_error("not implemented " + std::to_string(c));
}

void Lexer::state_numeric_literal() {
	if(c == '0') discard();
	throw std::runtime_error("not implemented " + std::to_string(c));
}

void Lexer::state_char_literal() {
	if(c == '0') discard();
	throw std::runtime_error("not implemented " + std::to_string(c));
}

void Lexer::state_identifier() {
	if(c == '0') discard();
	throw std::runtime_error("not implemented " + std::to_string(c));
}

void Lexer::state_symbol() {
	if(c == '0') discard();
	throw std::runtime_error("not implemented " + std::to_string(c));
}

} // namespace ee::lex
