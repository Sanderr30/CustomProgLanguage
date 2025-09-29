#include <runtime/function/function.h>


FunctionalObject::FunctionalObject(std::vector<std::string> params
                                , const std::vector<Statement>* body
                                , Enviroment* clsr)
    : parameters(std::move(params))
    , f_body(body)
    , closure(clsr)
    , native(nullptr)
{}


FunctionalObject::FunctionalObject(NativeFn fn)
    : parameters()
    , f_body(nullptr)
    , closure(nullptr)
    , native(std::move(fn))
{}
