#pragma once

#include <stdexcept>


class EnviromentError : public std::runtime_error {
public:
    static constexpr const char* kUndefinedVariable = "Undefined variable: ";

    EnviromentError(const std::string&);
};
