#include <runtime/enviroment/errors/env_errors.h>


EnviromentError::EnviromentError(const std::string& message)
    : std::runtime_error(message)
{}
