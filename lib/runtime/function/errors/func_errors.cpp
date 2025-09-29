#include <runtime/function/errors/func_errors.h>


ReturnException::ReturnException(Value val)
    : std::runtime_error(kReturnException)
    , value(val)
{}

BreakException::BreakException()
    : std::runtime_error(kBreakException)
{}

ContinueException::ContinueException()
    : std::runtime_error(kContinueException)
{}
