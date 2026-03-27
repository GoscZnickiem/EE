#include "compiler_unit/Emitter.hpp"
#include "lexer/Lexer.hpp"
#include "types/Character.hpp"
#include "types/Token.hpp"
#include "types/Types.hpp"

#include <cstddef>
#include <gtest/gtest.h>
#include <ranges>
#include <vector>

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
			.c = data[index++], .meta_data{.line = 1, .column = column++}
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

TEST(lex_Lexer, EmitsStringLiteral) {
	ee::String ref{R"("This is string literal \" ")"};
	ee::Token res;
	test::Reader reader{ref};
	ee::lex::Lexer lexer;

	lexer << reader;

	lexer >> res;

	EXPECT_EQ(res.type, ee::Token::Type::STRING_LITERAL);
	EXPECT_EQ(res.data, ref);
}

TEST(lex_Lexer, EmitsNumericLiteral) {
	ee::String ref{"0XA55BffI64"};
	test::Reader reader{ref};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().data, ref);
}

TEST(lex_Lexer, EmitsNumericLiterals) {
	std::vector<ee::String> refs{"23",	  "0B010110", "0o23577i", "0XA55BffI64", "2137Usize", "0000000003",
								 ".9F32", "17.",	  "239.199f", "10.f64",		 "5e10",	  "0.2E-30F64"};
	auto ref = refs | std::views::join_with(' ') | std::ranges::to<ee::String>();
	test::Reader reader{ref};
	ee::lex::Lexer lexer;

	lexer << reader;

	std::size_t i = 0;
	while (lexer) {
		ee::Token res;
		lexer >> res;
		EXPECT_EQ(res.type, ee::Token::Type::NUMERIC_LITERAL);
		EXPECT_EQ(res.data, refs[i++]);
	}
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

TEST(lex_Lexer, EmitsMetaData) {
	test::Reader reader{"Token   ;   \"Hello\";"};
	ee::lex::Lexer lexer;

	lexer << reader;

	EXPECT_EQ(lexer.emit().meta_data, (ee::Token::MetaData{.line_start = 1, .column_start = 1, .line_end = 1, .column_end = 5}));
	EXPECT_EQ(lexer.emit().meta_data, (ee::Token::MetaData{.line_start = 1, .column_start = 9, .line_end = 1, .column_end = 9}));
	EXPECT_EQ(lexer.emit().meta_data, (ee::Token::MetaData{.line_start = 1, .column_start = 13, .line_end = 1, .column_end = 19}));
	EXPECT_EQ(lexer.emit().meta_data, (ee::Token::MetaData{.line_start = 1, .column_start = 20, .line_end = 1, .column_end = 20}));
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
