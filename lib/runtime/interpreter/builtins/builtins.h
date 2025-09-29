#pragma once

#include <functional>
#include <unordered_map>
#include <string>

#include <runtime/value/value.h>
#include <runtime/function/function.h>
#include <runtime/enviroment/enviroment.h>


class Enviroment;


class BuiltinRegistry {
public:
    using BuiltinFunction = std::function<Value(const std::vector<Value>&)>;

    static BuiltinRegistry& Get() {
        static BuiltinRegistry instance;
        return instance;
    }

    void RegisterAll(Enviroment&, std::ostream&, std::istream&);

private:
    void RegisterIOFunctions(Enviroment&, std::ostream&, std::istream&);
    void RegisterUtilityFunctions(Enviroment&);
    void RegisterMathFunctions(Enviroment&);
    void RegisterStringFunctions(Enviroment&);
    void RegisterArrayFunctions(Enviroment&);
    void RegisterSystemFunctions(Enviroment&);

private:
    void CheckArgumentCount(const std::vector<Value>&
                , std::size_t, const std::string&);

    void CheckMinArgumentCount(const std::vector<Value>&
                , std::size_t, const std::string&);

    double ExtractNumber(const Value&, const std::string&);

    std::string ExtractString(const Value&, const std::string&);

    Value::Array ExtractArray(const Value&, const std::string&);

    Value CreateStringArray(const std::vector<std::string>&);

    void Register(const std::string&, BuiltinFunction);

    void AddToEnvironment(Enviroment&, const std::string&);

private:
    std::unordered_map<std::string, BuiltinFunction> functions_;
};
