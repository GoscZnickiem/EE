#ifndef EE_LIB_FILE_READER
#define EE_LIB_FILE_READER

#include "compiler_unit/Emitter.hpp"
#include "types/Character.hpp"

#include <filesystem>
#include <fstream>
#include <types/Types.hpp>

namespace ee::file {

class Reader : cmpu::Emitter<Character> {
private:
	std::ifstream source;
	std::filesystem::path source_file;

	Character::MetaData current_metadata{};

public:
	Character emit() override;
	[[nodiscard]] bool done() override;

	void set_source(const std::filesystem::path& file_name);
};

inline Reader& operator>>(const std::filesystem::path& file_name, Reader& reader) {
	reader.set_source(file_name);
	return reader;
}

} // namespace ee::file

#endif // !EE_LIB_FILE_READER
