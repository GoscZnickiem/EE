#ifndef EE_LIB_PARSER_BRACER
#define EE_LIB_PARSER_BRACER

#include <compiler_unit/Unit.hpp>
#include <optional>
#include <types/Character.hpp>
#include <types/Token.hpp>
#include <variant>
#include <vector>

namespace ee::prs {

struct Block;
using BlockToken = std::variant<Block, Token>;

struct Block {
	std::vector<BlockToken> contents;
};

class Bracer : public cmpu::Unit<Token, BlockToken> {
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

} // namespace ee::prs

#endif // !EE_LIB_PARSER_BRACER
