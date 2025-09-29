#pragma once

#include <stdexcept>

#include "../token/token.h"


class LexerError : public std::runtime_error {
public:
    static constexpr const char* kLexerErrorLine = "Lexer error at line ";
    static constexpr const char* kUnexpectedChar = "Unexpected character: ";
    static constexpr const char* kUntermStringLit = "Unterminated string literal";
    static constexpr const char* kColumnCntr = ", column ";
    static constexpr const char* kEmptyNumberLit = "Empty number literal";
    static constexpr const char* kInvalidTrailingSymb = "Invalid trailing symbol in: ";
    static constexpr const char* kInvalidExponent = "Invalid exponent in: ";
    static constexpr const char* kUntermSequence = "Unterminated escape sequence";
    static constexpr const char* kUnknownEscape = "Unknown escape: \\";

public:
    LexerError(const Token&, const std::string&);

private:
    Token token_;
};
