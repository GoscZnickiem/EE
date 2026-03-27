#include "types/Character.hpp"

#include <string>

namespace ee {

std::string Character::to_string() const {
	return std::string("{'") + c + "', " + std::to_string(meta_data.line) + ":" + std::to_string(meta_data.column) + "}";
}

}; // namespace ee
