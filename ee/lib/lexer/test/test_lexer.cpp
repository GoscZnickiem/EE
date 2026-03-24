#include "compiler_unit/Emitter.hpp"
#include "lexer/Lexer.hpp"
#include "types/Character.hpp"
#include "types/Types.hpp"

#include <cstddef>
#include <gtest/gtest.h>

namespace test {

class Reader : public ee::cmpu::Emitter<ee::Character> {
private:
	ee::String data;
	std::size_t column{1};
	std::size_t index{0};

public:
	explicit Reader(ee::String input) : data(std::move(input)) {}

	ee::Character emit() override {
		return {
			.c = data[index++], .meta_data{.line = 0, .column = column++}
		};
	}

	[[nodiscard]] bool done() override {
		return index == data.length();
	}
};

} // namespace test

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

TEST(lex_Lexer, EmitsSemicolon) {
	test::Reader reader{";"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::SEMICOLON);
}

TEST(lex_Lexer, EmitsComma) {
	test::Reader reader{","};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::COMMA);
}

TEST(lex_Lexer, EmitsBraceL) {
	test::Reader reader{"{"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_L);
}

TEST(lex_Lexer, EmitsBraceR) {
	test::Reader reader{"}"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_R);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
