#include "lex_errors.h"


LexerError::LexerError(const Token& token, const std::string& msg)
    : token_(token)
    , std::runtime_error(kLexerErrorLine
                        + std::to_string(token.line)
                        + kColumnCntr
                        + std::to_string(token.column)
                        + " "
                        + msg)
{}
