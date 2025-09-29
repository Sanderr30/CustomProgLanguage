#pragma once

#include <stdexcept>


class EvaluatorErrors : std::runtime_error {
public:
    static constexpr const char* kInvalidOperand = "Operand is not a number or bool";
    static constexpr const char* kBadOperandsForBinaryOperation = "Bad operands for binary operation";
    static constexpr const char* kCallOfNonFunction = "Call of non-function";
    static constexpr const char* kUndefinedVariable = "string index out of range";
    static constexpr const char* kArrayIndexOutOfRange = "Array index out of range";
    static constexpr const char* kInvalidArrayIndex = "Indexing non-indexable type";
    static constexpr const char* kInvalidSlice = "Slicing non-sliceable type";
    static constexpr const char* kUnsupportedBinaryOperation = "Unsupported binary operation";
    static constexpr const char* kUnsupportedUnaryOperation = "Unsupported unary operation";

public:
    EvaluatorErrors(const std::string&);
};
