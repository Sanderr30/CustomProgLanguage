#include "errors/ev_errors.h"
#include <runtime/evaluator/operations/register.h>


void OperationRegistry::AddBinary(TokenType op, BinaryHandler handler) {
    binary_ops_[op] = std::move(handler);
}


void OperationRegistry::AddUnary(TokenType op, UnaryHandler handler) {
    unary_ops_[op] = std::move(handler);
}


Value OperationRegistry::ExecuteBinary(TokenType op, const Value& left, const Value& right) const {
    if (auto it = binary_ops_.find(op); it != binary_ops_.end()) {
        return it->second(left, right);
    }
    throw EvaluatorErrors(EvaluatorErrors::kUnsupportedBinaryOperation);
}


Value OperationRegistry::ExecuteUnary(TokenType op, const Value& operand) const {
    if (auto it = unary_ops_.find(op); it != unary_ops_.end()) {
        return it->second(operand);
    }
    throw EvaluatorErrors(EvaluatorErrors::kUnsupportedBinaryOperation);
}


bool OperationRegistry::SupportsBinary(TokenType op) const {
    return binary_ops_.contains(op);
}


bool OperationRegistry::SupportsUnary(TokenType op) const {
    return unary_ops_.contains(op);
}
