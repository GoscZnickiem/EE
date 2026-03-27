#ifndef EE_LIB_TYPES_CHARACTER
#define EE_LIB_TYPES_CHARACTER

#include "types/Types.hpp"

#include <cstddef>
#include <ostream>
#include <string>

namespace ee {

class Character {
public:
	struct MetaData {
		std::size_t line;
		std::size_t column;
	};

	Char c;
	MetaData meta_data;

	[[nodiscard]] Char operator*() const noexcept {
		return c;
	}

	[[nodiscard]] std::string to_string() const;
};

inline std::ostream& operator<<(std::ostream& s, Character c) {
	return s << c.to_string();
}

} // namespace ee

#endif // !EE_LIB_TYPES_CHARACTER
