#include "compiler_unit/Emitter.hpp"
#include "lexer/Lexer.hpp"
#include "types/Character.hpp"
#include "types/Token.hpp"
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

TEST(lex_Lexer, EmitsName) {
	ee::String ref{"identifier0_1name"};
	ee::Token res;
	test::Reader reader{ref};
	ee::lex::Lexer lexer;

	lexer << reader;

	lexer >> res;

	EXPECT_EQ(res.type, ee::Token::Type::NAME);
	EXPECT_EQ(res.data, ref);
}

TEST(lex_Lexer, EmitsRawStringLiteral) {
	ee::String ref{R"(r"This is raw string literal \ \")"};
	ee::Token res;
	test::Reader reader{ref};
	ee::lex::Lexer lexer;

	lexer << reader;

	lexer >> res;

	EXPECT_EQ(res.type, ee::Token::Type::STRING_LITERAL);
	EXPECT_EQ(res.data, ref);
}

TEST(lex_Lexer, EmitsTokensSequentially) {
	test::Reader reader{";,{}"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::SEMICOLON);
	EXPECT_EQ(lexer.emit().type, ee::Token::Type::COMMA);
	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_L);
	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_R);
}

TEST(lex_Lexer, IgnoresWhitespaces) {
	test::Reader reader{"   \t \n ;  \t \n ,"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::SEMICOLON);
	EXPECT_EQ(lexer.emit().type, ee::Token::Type::COMMA);
}

TEST(lex_Lexer, IgnoresLineComment) {
	test::Reader reader{"{// This comment will be completely ignored ; \n}"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_L);
	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_R);
}

TEST(lex_Lexer, IgnoresBlockComment) {
	test::Reader reader{"{/* This comment will be completely ignored * ; */}"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_L);
	EXPECT_EQ(lexer.emit().type, ee::Token::Type::BRACE_R);
}

TEST(lex_Lexer, IsDoneAfterLastToken) {
	test::Reader reader{";;;"};
	ee::lex::Lexer lexer;

	lexer << reader;

	lexer.emit();
	lexer.emit();
	lexer.emit();

	EXPECT_TRUE(lexer.done());
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
