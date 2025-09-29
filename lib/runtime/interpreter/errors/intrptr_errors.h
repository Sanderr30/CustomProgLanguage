#pragma once

#include <stdexcept>


class InterpreterError : std::runtime_error {
public:
    static constexpr const char* kCanOnlyIterateArrays = "Can only iterate arrays";
    static constexpr const char* kUnknownError = "Interpreter error: unknown\n";

public:
    InterpreterError(const std::string&);
};
