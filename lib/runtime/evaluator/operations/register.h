#pragma once

#include <functional>

#include <runtime/value/value.h>
#include <runtime/evaluator/errors/ev_errors.h>
#include <token/token.h>


class OperationRegistry {
public:
    using BinaryHandler = std::function<Value(const Value&, const Value&)>;
    using UnaryHandler = std::function<Value(const Value&)>;

    static OperationRegistry& Get() {
        static OperationRegistry instance;
        return instance;
    }

    void AddBinary(TokenType, BinaryHandler);

    void AddUnary(TokenType, UnaryHandler);

    Value ExecuteBinary(TokenType, const Value&, const Value&) const;

    Value ExecuteUnary(TokenType, const Value&) const;

    bool SupportsBinary(TokenType) const;

    bool SupportsUnary(TokenType) const;

private:
    std::unordered_map<TokenType, BinaryHandler> binary_ops_;
    std::unordered_map<TokenType, UnaryHandler> unary_ops_;
};
