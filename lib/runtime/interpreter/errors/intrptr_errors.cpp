#include <runtime/interpreter/errors/intrptr_errors.h>


InterpreterError::InterpreterError(const std::string& message)
    : std::runtime_error(message)
{}
