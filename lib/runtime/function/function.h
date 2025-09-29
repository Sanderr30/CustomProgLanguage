#pragma once

#include <functional>
#include <vector>
#include <string>

#include <runtime/value/value.h>
#include <vls_and_sttmnts.h>
#include <runtime/function/errors/func_errors.h>


class Enviroment;


struct FunctionalObject {
    using NativeFn = std::function<Value(const std::vector<Value>&)>;

    std::vector<std::string> parameters;
    const std::vector<Statement>* f_body;
    Enviroment* closure;
    NativeFn native;

    FunctionalObject(std::vector<std::string>
                    , const std::vector<Statement>*
                    , Enviroment*);

    FunctionalObject(NativeFn fn);
};
