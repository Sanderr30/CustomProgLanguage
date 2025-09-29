#pragma once

#include <stdexcept>


class BuiltinError : public std::runtime_error {
public:
    static constexpr const char* kExpectedNumericArgument = "() expects numeric argument";
    static constexpr const char* kExpectedStringArgument = "() expects string argument";
    static constexpr const char* kExpectedArrayArgument = "() expects array argument";
    static constexpr const char* kArgumentMustBeStringOrArray = "len() argument must be string or array";
    static constexpr const char* kSqrtOfNegativeNumber = "sqrt() of negative number";
    static constexpr const char* kRndOfNegativeNumber = "rnd() argument must be positive";
    static constexpr const char* kSplitDelimiterCannotBeEmpty = "split() delimiter cannot be empty";
    static constexpr const char* kReplaceOldStringCannotBeEmpty = "replace() old string cannot be empty";
    static constexpr const char* kRangeInvalidArguments = "range() expects 1, 2 or 3 arguments";
    static constexpr const char* kRangeStepCannotBeZero = "range() step cannot be zero";
    static constexpr const char* kPopFromEmptyArray = "pop() from empty array";
    static constexpr const char* kInsertIndexOutOfRange = "insert() index out of range";
    static constexpr const char* kRemoveIndexOutOfRange = "remove() index out of range";
    static constexpr const char* kRndArgumentMustBePositive = "rnd() argument must be positive";
public:
    BuiltinError(const std::string&);
};
