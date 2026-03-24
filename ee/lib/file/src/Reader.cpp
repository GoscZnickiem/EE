#include "file/Reader.hpp"

#include "types/Character.hpp"
#include "types/Types.hpp"

#include <filesystem>
#include <ios>
#include <stdexcept>
#include <string>

namespace ee::file {

Reader::emitted_type Reader::emit() {
	if (!source.is_open()) throw std::runtime_error("No source file");

	const int input = source.get();

	if (input == std::char_traits<char>::eof()) throw std::runtime_error("End of file reached");

	const Char c = static_cast<Char>(input);

	if (c == '\n') {
		current_metadata = {.line = current_metadata.line + 1, .column = 1};
	} else {
		current_metadata.column++;
	}

	return Character{.c = c, .meta_data = current_metadata};
}

bool Reader::done() {
	if (!source.is_open()) return true;

	return source.peek() == std::char_traits<char>::eof();
}

void Reader::set_source(const std::filesystem::path& file_name) {
	source_file = file_name;
	source.close();
	source.clear();
	source.open(file_name, std::ios::in);
	current_metadata = {.line = 1, .column = 0};

	if (!source.is_open()) throw std::runtime_error("Failed to open file: " + source_file.string());
}

} // namespace ee::file
