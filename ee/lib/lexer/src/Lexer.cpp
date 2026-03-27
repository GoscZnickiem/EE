#include "lexer/Lexer.hpp"

#include "LexerStateMachine.hpp"

#include <cassert>
#include <types/Character.hpp>
#include <types/Token.hpp>
#include <types/Types.hpp>
#include <utility>

namespace ee::lex {

Token Lexer::read_token() {
	Token token{.type = Token::Type::INVALID, .data{}, .meta_data{}};
	StateMachine automaton{token.data};

	while (true) {
		if (!character) {
			if (input_empty()) {
				auto result = automaton.pull_last();
				if (result) token.type = *result;
				state = result ? State::LAST : State::DONE;
				break;
			}
			character = get();
		}

		const Character cc = *character;
		const Char c = *cc;
		auto [result_char, result_token] = automaton.step(c);

		if (result_char == StateMachine::CharResult::ACCEPT) {
			if (token.data.empty()) {
				token.meta_data.line_start = cc.meta_data.line;
				token.meta_data.column_start = cc.meta_data.column;
			}
			token.meta_data.line_end = cc.meta_data.line;
			token.meta_data.column_end = cc.meta_data.column;
			token.data += c;
		} else if (result_char == StateMachine::CharResult::CLEAR) {
			token.data.clear();
		}
		if (result_char != StateMachine::CharResult::LEAVE) {
			character.reset();
		}

		if (!result_token) continue;
		token.type = *result_token;
		break;
	}
	return token;
}

Lexer::emitted_type Lexer::emit() {
	if (state == State::INIT) {
		buffer = read_token();
		state = State::READY;
	} else if (state == State::LAST) {
		state = State::DONE;
		return buffer;
	}
	return std::exchange(buffer, read_token());
}

bool Lexer::done() {
	return state == State::DONE;
}

} // namespace ee::lex
