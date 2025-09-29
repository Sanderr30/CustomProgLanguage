#pragma once

#include <stdexcept>

#include <runtime/value/value.h>


class ReturnException : public std::runtime_error {
public:
    static constexpr const char* kReturnException = "return";
public:
    Value value;
    ReturnException(Value);
};

class BreakException : public std::runtime_error {
public:
    static constexpr const char* kBreakException = "break";
public:
    BreakException();
};

class ContinueException : public std::runtime_error {
public:
    static constexpr const char* kContinueException = "continue";
public:
    ContinueException();
};
