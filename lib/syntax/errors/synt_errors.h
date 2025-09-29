#pragma once

#include <stdexcept>

#include <token/token.h>


class SyntaxError : public std::runtime_error {
public:
    static constexpr const char* kSyntaxErrorLine = "Syntax error at line ";
    static constexpr const char* kColumnCntr = ", column ";
    static constexpr const char* kExpectedToken = "Expected token not found";
    static constexpr const char* kExpectedIdentifierInFor = "Expected identifier in for loop";
    static constexpr const char* kInvalidAssignmentTarget = "Invalid assignment target";
    static constexpr const char* kExpectedParameterName = "Expected parameter name";
    static constexpr const char* kUnexpectedTokenInPrimary = "Unexpected token in primary expression";
    static constexpr const char* kUnexpectedEOF = "Unexpected end of file";
    static constexpr const char* kExpectedRightParen = "Expected ')' to close function call";
    static constexpr const char* kExpectedRightBracket = "Expected ']' to close array index";
    static constexpr const char* kExpectedExpression = "Expected expression";
    static constexpr const char* kMissingRightOperand = "Missing right operand for binary operator";

public:
    SyntaxError(const Token&, const std::string&);

private:
    Token token_;
};
