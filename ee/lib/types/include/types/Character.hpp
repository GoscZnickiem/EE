#ifndef EE_LIB_TYPES_CHARACTER
#define EE_LIB_TYPES_CHARACTER

#include <cstddef>
#include <types/Types.hpp>

namespace ee {

class Character {
public:
	struct MetaData {
		std::size_t line;
		std::size_t column;
	};

	Char c;
	MetaData meta_data;

	[[nodiscard]] Char operator*() const {
		return c;
	}
};

} // namespace ee

#endif // !EE_LIB_TYPES_CHARACTER
