#ifndef EE_LIB_LEXER_EXCEPTION
#define EE_LIB_LEXER_EXCEPTION

#include <stdexcept>
#include <string>

namespace ee::lex::err {

class LexerError : public std::runtime_error {
public:
	constexpr explicit LexerError(const std::string& what_arg) : runtime_error(what_arg) {}

	constexpr explicit LexerError(const char* what_arg) : runtime_error(what_arg) {}
};

class UnterminatedStringError : public LexerError {
public:
	constexpr explicit UnterminatedStringError(const std::string& what_arg) : LexerError(what_arg) {}

	constexpr explicit UnterminatedStringError(const char* what_arg) : LexerError(what_arg) {}
};

class UnterminatedCharacterLiteralError : public LexerError {
public:
	constexpr explicit UnterminatedCharacterLiteralError(const std::string& what_arg) : LexerError(what_arg) {}

	constexpr explicit UnterminatedCharacterLiteralError(const char* what_arg) : LexerError(what_arg) {}
};

class IncompleteTokenError : public LexerError {
public:
	constexpr explicit IncompleteTokenError(const std::string& what_arg) : LexerError(what_arg) {}

	constexpr explicit IncompleteTokenError(const char* what_arg) : LexerError(what_arg) {}
};

class InvalidDigitError : public LexerError {
public:
	constexpr explicit InvalidDigitError(const std::string& what_arg) : LexerError(what_arg) {}

	constexpr explicit InvalidDigitError(const char* what_arg) : LexerError(what_arg) {}
};

class ExpectedExponentialFloatNotation : public LexerError {
public:
	constexpr explicit ExpectedExponentialFloatNotation(const std::string& what_arg) : LexerError(what_arg) {}

	constexpr explicit ExpectedExponentialFloatNotation(const char* what_arg) : LexerError(what_arg) {}
};

class ExpectedNumericLiteralSufix : public LexerError {
public:
	constexpr explicit ExpectedNumericLiteralSufix(const std::string& what_arg) : LexerError(what_arg) {}

	constexpr explicit ExpectedNumericLiteralSufix(const char* what_arg) : LexerError(what_arg) {}
};

} // namespace ee::lex::err

#endif // !EE_LIB_LEXER_EXCEPTION
