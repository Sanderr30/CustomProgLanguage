#include <runtime/interpreter/builtins/errors/bltns_errors.h>


BuiltinError::BuiltinError(const std::string& message)
    : std::runtime_error(message)
{}
