#pragma once

#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <iostream>


class Enviroment;


struct NilType { };


struct FunctionalObject;


class Value {
public:
    using Array = std::vector<std::shared_ptr<Value>>;
    using FuncPtr = std::shared_ptr<FunctionalObject>;

public:
    std::variant<double, std::string
                , bool, NilType
                , Array, FuncPtr> data;

    Value();

    Value(double);

    Value(const std::string&);

    Value(const char* str);

    explicit Value(bool val)
        : data(val)
    {}

    Value(NilType);

    Value(const Array&);

    Value(FuncPtr);

public:
    bool IsNumber() const;

    bool IsString() const;

    bool IsBool() const;

    bool IsNil() const;

    bool IsList() const;

    bool IsFunction() const;

public:
    double AsNumber() const;

    bool AsBool() const;

    const std::string& AsString() const;

    const Array& AsList() const;

    FuncPtr AsFunction() const;

    Array& AsList();

    std::string ToString() const;
    friend std::ostream& operator<<(std::ostream&, const Value&);
};
