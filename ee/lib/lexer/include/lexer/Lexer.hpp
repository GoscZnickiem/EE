#ifndef EE_LIB_LEXER_LEXER
#define EE_LIB_LEXER_LEXER

#include <compiler_unit/Unit.hpp>
#include <optional>
#include <types/Character.hpp>
#include <types/Token.hpp>

namespace ee::lex {

class Lexer : public cmpu::Unit<Character, Token> {
public:
	emitted_type emit() override;

	[[nodiscard]] bool done() override;

private:
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
