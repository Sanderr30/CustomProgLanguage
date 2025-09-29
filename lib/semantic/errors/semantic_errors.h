#pragma once


struct ErrorMsgHandler {
    static constexpr const char*
    kDefaultMessage = "Semantic error";
    static constexpr const char*
    kVariableAlreadyDeclared = "Variable already declared in current scope: ";
    static constexpr const char*
    kUndefinedVariable = "Undefined variable: ";
    static constexpr const char*
    kDuplicatedParameter = "Duplicate parameter name: ";
    static constexpr const char*
    kFailingDeclaration = "Failed to declare variable: ";
    static constexpr const char*
    kInvalidBinaryOperation = "Invalid binary operation for types in expression";
    static constexpr const char*
    kInvalidUnaryOperation = "Invalid unary operation for type in expression";
    static constexpr const char*
    kWrongNumberOfArguments = "Wrong number of arguments for function: ";
    static constexpr const char*
    kTypeMismatchInArgument = "Type mismatch in function argument for: ";
    static constexpr const char*
    kAttemptingToCallNonCallableExpression = "Attempting to call non-callable expression";
    static constexpr const char*
    kAttemptingToIndexNonIndexableType = "Attempting to index non-indexable type";
    static constexpr const char*
    kIndexMustBeNumber = "Index must be a number";
    static constexpr const char*
    kAttemptingToSliceNonSliceableType = "Attempting to slice non-sliceable type";
    static constexpr const char*
    kSliceStartIndexMustBeNumber = "Slice start index must be a number";
    static constexpr const char*
    kTypeMismatchInSlice = "Type mismatch in slice";
};
