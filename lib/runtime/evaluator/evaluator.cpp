#include <algorithm>
#include <cmath>
#include <memory>

#include <runtime/evaluator/evaluator.h>
#include <runtime/interpreter/interpreter.h>
#include <runtime/evaluator/errors/ev_errors.h>
#include <runtime/evaluator/operations/handlers.h>
#include <runtime/evaluator/operations/register.h>
#include <semantic.h>


static void EnsInitialized() {
    static bool initialized = false;
    if (!initialized) {
        Initialize();
        initialized = true;
    }
}

ExpressionEvaluator::ExpressionEvaluator(Interpreter* interpreter, Enviroment* env)
    : interpreter_(interpreter)
    , env_(env)
{
    EnsInitialized();
}


Value ExpressionEvaluator::operator()(const NumberExpression& expr) const {
    return Value(expr.value);
}


Value ExpressionEvaluator::operator()(const StringExpression& expr) const {
    return Value(expr.value);
}


Value ExpressionEvaluator::operator()(const BoolExpression& expr) const {
    return Value(expr.value);
}


Value ExpressionEvaluator::operator()(const NilExpression&) const {
    return Value(NilType{});
}


Value ExpressionEvaluator::operator()(const VariableExpression& expr) const {
    return env_->Get(expr.name);
}


Value ExpressionEvaluator::operator()(const UnaryExpression& expr) const {
    Value operand = interpreter_->ParseNode(*expr.rhs, env_);
    auto& registry = OperationRegistry::Get();
    if (registry.SupportsUnary(expr.operation)) {
        return registry.ExecuteUnary(expr.operation, operand);
    }
    throw EvaluatorErrors(EvaluatorErrors::kBadOperandsForBinaryOperation);
}


Value ExpressionEvaluator::operator()(const BinaryExpression& expr) const {
    if (expr.operation == TokenType::and_) {
        Value left = interpreter_->ParseNode(*expr.lhs, env_);
        if (!interpreter_->IsTrue(left)) {
            return left;
        }
        return interpreter_->ParseNode(*expr.rhs, env_);
    }
    if (expr.operation == TokenType::or_) {
        Value left = interpreter_->ParseNode(*expr.lhs, env_);
        if (interpreter_->IsTrue(left)) {
            return left;
        }
        return interpreter_->ParseNode(*expr.rhs, env_);
    }

    Value left = interpreter_->ParseNode(*expr.lhs, env_);
    Value right = interpreter_->ParseNode(*expr.rhs, env_);

    if (expr.operation == TokenType::double_eq_) {
        return Value(interpreter_->IsEqual(left, right));
    }
    if (expr.operation == TokenType::not_eq_) {
        return Value(!interpreter_->IsEqual(left, right));
    }

    auto& registry = OperationRegistry::Get();
    if (registry.SupportsBinary(expr.operation)) {
        return registry.ExecuteBinary(expr.operation, left, right);
    }

    throw EvaluatorErrors(EvaluatorErrors::kBadOperandsForBinaryOperation);
}


Value ExpressionEvaluator::operator()(const CallableExpression& expr) const {
    Value callable = interpreter_->ParseNode(*expr.callable, env_);

    if (!std::holds_alternative<Value::FuncPtr>(callable.data)) {
        throw EvaluatorErrors(EvaluatorErrors::kCallOfNonFunction);
    }

    auto function = std::get<Value::FuncPtr>(callable.data);
    std::vector<Value> arguments;
    arguments.reserve(expr.f_arguments.size());

    for (const auto& arg : expr.f_arguments) {
        arguments.push_back(interpreter_->ParseNode(*arg, env_));
    }

    return interpreter_->PerformFunction(function, arguments);
}


Value ExpressionEvaluator::operator()(const ListExpression& expr) const {
    Value::Array array;
    array.reserve(expr.elements.size());

    for (const auto& element : expr.elements) {
        array.push_back(std::make_shared<Value>(interpreter_->ParseNode(*element, env_)));
    }

    return Value(array);
}


Value ExpressionEvaluator::operator()(const FunctionExpression& expr) const {
    auto function_obj = std::make_shared<FunctionalObject>(
        expr.parameters, &expr.f_body, env_
    );
    return Value(function_obj);
}


Value ExpressionEvaluator::operator()(const AssignExpression& expr) const {
    Value value = interpreter_->ParseNode(*expr.rhs, env_);

    if (!env_->Assign(expr.name, value)) {
        env_->Define(expr.name, value);
    }

    return value;
}


Value ExpressionEvaluator::operator()(const IndexExpression& expr) const {
    Value object = interpreter_->ParseNode(*expr.object, env_);
    Value index_value = interpreter_->ParseNode(*expr.index, env_);
    int index = static_cast<int>(std::get<double>(index_value.data));

    auto normalize_index = [](int idx, int size) constexpr -> int {
        return idx < 0 ? idx + size : idx;
    };

    if (auto* str = std::get_if<std::string>(&object.data)) {
        int size = static_cast<int>(str->size());
        int normalized_idx = normalize_index(index, size);

        if (normalized_idx < 0 || normalized_idx >= size) {
            throw EvaluatorErrors(EvaluatorErrors::kUndefinedVariable);
        }

        return Value(std::string(1, (*str)[normalized_idx]));
    }

    if (auto* array = std::get_if<Value::Array>(&object.data)) {
        int size = static_cast<int>(array->size());
        int normalized_idx = normalize_index(index, size);

        if (normalized_idx < 0 || normalized_idx >= size) {
            throw EvaluatorErrors(EvaluatorErrors::kArrayIndexOutOfRange);
        }

        return *(*array)[normalized_idx];
    }

    throw EvaluatorErrors(EvaluatorErrors::kInvalidArrayIndex);
}


Value ExpressionEvaluator::operator()(const SliceExpression& expr) const {
    Value object = interpreter_->ParseNode(*expr.object, env_);

    auto normalize_and_clamp = [](int idx, int size) constexpr -> int {
        int normalized = idx < 0 ? idx + size : idx;
        return std::clamp(normalized, 0, size);
    };

    int from = 0;
    if (expr.from_s) {
        from = static_cast<int>(std::get<double>(
            interpreter_->ParseNode(*expr.from_s, env_).data));
    }

    if (auto* str = std::get_if<std::string>(&object.data)) {
        int size = static_cast<int>(str->size());

        int to = expr.to_s ?
                static_cast<int>(std::get<double>(
                interpreter_->ParseNode(*expr.to_s, env_).data))
            :
                size;

        from = normalize_and_clamp(from, size);
        to = normalize_and_clamp(to, size);

        return Value(str->substr(from, to - from));
    }

    if (auto* array = std::get_if<Value::Array>(&object.data)) {
        int size = static_cast<int>(array->size());

        int to = expr.to_s
            ? static_cast<int>(std::get<double>(interpreter_->ParseNode(*expr.to_s, env_).data))
            : size;

        from = normalize_and_clamp(from, size);
        to = normalize_and_clamp(to, size);

        Value::Array result;
        result.reserve(to - from);

        for (int i = from; i < to; ++i) {
            result.push_back(std::make_shared<Value>(*(*array)[i]));
        }

        return Value(result);
    }

    throw EvaluatorErrors(EvaluatorErrors::kInvalidSlice);
}
