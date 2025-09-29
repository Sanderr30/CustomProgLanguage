#include "val_errors.h"

ValueErrors::ValueErrors(const std::string& message)
    : std::runtime_error(message)
{}
