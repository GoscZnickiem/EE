#include "types/Token.hpp"

#include <string>

namespace ee {

namespace {

std::string type_to_string(Token::Type type) {
	switch (type) {
		case Token::Type::INVALID:               return "Invalid";
		case Token::Type::NAME:                  return "Name";
		case Token::Type::SYMBOL:                return "Symbol";
		case Token::Type::NUMERIC_LITERAL:       return "Numeric literal";
		case Token::Type::STRING_LITERAL:        return "String literal";
		case Token::Type::CHAR_LITERAL:          return "Char literal";
		case Token::Type::SEMICOLON:             return "Semicolon";
		case Token::Type::COMMA:                 return "Comma";
		case Token::Type::BRACE_L:               return "Left brace";
		case Token::Type::BRACE_R:				 return "Right brace";
	}
}

} // namespace

std::string Token::to_string() const {
	return std::string("{'") + data + "', " + type_to_string(type) + ", "
		 + std::to_string(meta_data.line_start) + ":" + std::to_string(meta_data.column_start) + " - "
		 + std::to_string(meta_data.line_end) + ":" + std::to_string(meta_data.column_end) + "}";
}

}; // namespace ee
