#include <runtime/evaluator/errors/ev_errors.h>

EvaluatorErrors::EvaluatorErrors(const std::string& message)
    : std::runtime_error(message)
{}
