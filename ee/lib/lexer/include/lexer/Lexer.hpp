#ifndef EE_LIB_LEXER_LEXER
#define EE_LIB_LEXER_LEXER

#include "lexer/Token.hpp"

#include <stream/stream.hpp>

namespace ee::lex {

class lexer : public stream::stream<Token> {
public:

};

} // namespace ee::lex

#endif // !EE_LIB_LEXER_LEXER
