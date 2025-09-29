#pragma once

#include <stdexcept>


class ValueErrors : public std::runtime_error {
public:
    static constexpr const char* kValueNotNumber = "Value is not a number";
    static constexpr const char* kValueNotString = "Value is not a string";
    static constexpr const char* kValueNotBool = "Value is not a boolean";
    static constexpr const char* kValueNotList = "Value is not a list";
    static constexpr const char* kValueNotFunction = "Value is not a function";

public:
    ValueErrors(const std::string&);
};
