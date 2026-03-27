#include <compiler_unit/Emitter.hpp>
#include <iostream>
#include <iterator>
#include <lexer/Lexer.hpp>
#include <stdexcept>
#include <types/Types.hpp>

class StdinSource : public ee::cmpu::Emitter<ee::Character> {
private:
	ee::String buffer;
	std::size_t pos = 0;
	std::size_t line = 1;
	std::size_t column = 1;

public:
	StdinSource() {
		buffer.assign(
			std::istreambuf_iterator<char>(std::cin),
			std::istreambuf_iterator<char>()
		);
	}

	ee::Character emit() override {
		if (done()) throw std::runtime_error("No more input");
		ee::Character::MetaData meta{.line = line, .column = column++};
		ee::Char c = buffer[pos++];
		if (c == '\n') {
			line++;
			column = 1;
		}
		return ee::Character{.c = c, .meta_data = meta};
	}

	bool done() override {
		return pos >= buffer.size();
	}
};

int main() {
	ee::lex::Lexer lexer;
	StdinSource src;

	lexer << src;

	while(lexer) std::cout << lexer.emit() << '\n';
}
