#pragma once

#include <unordered_map>
#include <string>

#include <runtime/value/value.h>
#include <runtime/enviroment/errors/env_errors.h>


class Enviroment {
public:
    Enviroment();
    Enviroment(Enviroment*);

public:
    bool Define(const std::string&, Value);

    bool Assign(const std::string&, Value);

    Value Get(const std::string&) const;

private:
    Enviroment* parent_ = nullptr;
    std::unordered_map<std::string, Value> values_;
};
