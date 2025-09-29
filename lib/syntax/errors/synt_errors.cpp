#include <errors/synt_errors.h>

SyntaxError::SyntaxError(const Token& token, const std::string& msg)
    : token_(token)
    , std::runtime_error(kSyntaxErrorLine
                        + std::to_string(token.line)
                        + kColumnCntr
                        + std::to_string(token.column)
                        + " "
                        + msg)
{}
