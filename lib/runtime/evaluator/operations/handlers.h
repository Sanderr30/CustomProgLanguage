#pragma once

#include <cmath>
#include <runtime/evaluator/errors/ev_errors.h>
#include <runtime/evaluator/operations/register.h>


constexpr double AsNumber(const Value&);

constexpr bool IsTrue(const Value&);

Value Add(const Value&, const Value&);

Value Substract(const Value&, const Value&);

Value Multiply(const Value&, const Value&);

Value Divide(const Value&, const Value&);

Value Mod(const Value&, const Value&);

Value PowerOf(const Value&, const Value&);


template<typename Comparator>
Value Compare(const Value& left, const Value& right, Comparator comp) {
    if (auto* str1 = std::get_if<std::string>(&left.data)) {
        if (auto* str2 = std::get_if<std::string>(&right.data)) {
            return Value(comp(*str1, *str2));
        }
    }
    return Value(comp(AsNumber(left), AsNumber(right)));
}

Value Less(const Value&, const Value&);

Value LessEqual(const Value&, const Value&);

Value Greater(const Value&, const Value&);

Value GreaterEqual(const Value&, const Value&);

Value Negate(const Value&);

Value LogicalNot(const Value&);

void Initialize();
