#include "compiler_unit/Emitter.hpp"
#include "lexer/Lexer.hpp"
#include "types/Character.hpp"
#include "types/Types.hpp"

#include <cstddef>
#include <gtest/gtest.h>

namespace test {

class FileReader : public ee::cmpu::Emitter<ee::Character> {
private:
	ee::String data;
	std::size_t column{1};
	std::size_t index{0};

public:
	explicit FileReader(ee::String input) : data(std::move(input)) {}

	ee::Character emit() override {
		return {
			.c = data[index++], .meta_data{.line = 0, .column = column++}
		};
	}

	[[nodiscard]] bool done() override {
		return index == data.length() - 1;
	}
};

} // namespace test

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

TEST(cmpu_Lexer, EmitsIdentifierToken) {
	ee::String ref = "testIdentifier";
	ee::Token res{};
	test::FileReader reader{ref};
	ee::lex::Lexer lexer;

	lexer << reader;

	lexer >> res;
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
