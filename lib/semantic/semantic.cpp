#include "errors/semantic_errors.h"
#include "vls_and_sttmnts.h"
#include <semantic.h>


inline void SemanticAnalizer::ErrorReport(const std::string& message
    , const std::string& details)
{
    error_stream_ << ErrorMsgHandler::kDefaultMessage << message;
    if (!details.empty()) {
        error_stream_ << details;
    }
    error_stream_ << std::endl;
}


SemanticAnalizer::SemanticAnalizer(std::ostream& error_stream)
    : error_stream_(error_stream)
{}


bool SemanticAnalizer::Analyse(const std::vector<Statement>& program) {
    symbol_table_.EnterScope();

    for (const auto& builtin : SymbolTable::kBuiltinFunctions) {
        symbol_table_.Declare(builtin);
    }

    bool success = ProcessStatements(program);
    symbol_table_.ExitScope();

    return success;
}


bool SemanticAnalizer::ProcessStatement(const Statement& statement) {
    return std::visit([this](const auto& stmt) {
        return ProcessStatementImpl(stmt);
    }, statement.value);
}


bool SemanticAnalizer::ProcessExpression(const Expression& expression) {
    return std::visit([this](const auto& expr) {
        return ProcessExpressionImpl(expr);
    }, expression.value);
}


SemanticType SemanticAnalizer::GetExpressionType(const Expression& expression) {
    return std::visit([this](const auto& expr) -> SemanticType {
        using T = std::decay_t<decltype(expr)>;

        if constexpr (std::is_same_v<T, NumberExpression>) {
            return SemanticType::Number;
        } else if constexpr (std::is_same_v<T, StringExpression>) {
            return SemanticType::String;
        } else if constexpr (std::is_same_v<T, BoolExpression>) {
            return SemanticType::Bool;
        } else if constexpr (std::is_same_v<T, NilExpression>) {
            return SemanticType::Nil;
        } else if constexpr (std::is_same_v<T, ListExpression>) {
            return SemanticType::List;
        } else if constexpr (std::is_same_v<T, FunctionExpression>) {
            return SemanticType::Function;
        } else if constexpr (std::is_same_v<T, VariableExpression>) {
            auto it = variable_types_.find(expr.name);
            return (it != variable_types_.end()) ? it->second : SemanticType::Unknown;
        } else if constexpr (std::is_same_v<T, BinaryExpression>) {
            auto left_type = GetExpressionType(*expr.lhs);
            auto right_type = GetExpressionType(*expr.rhs);
            return TypeSystem::GetBinaryResultType(expr.operation, left_type, right_type);
        } else if constexpr (std::is_same_v<T, UnaryExpression>) {
            auto operand_type = GetExpressionType(*expr.rhs);
            return TypeSystem::GetUnaryResultType(expr.operation, operand_type);
        } else {
            return SemanticType::Unknown;
        }
    }, expression.value);
}


bool SemanticAnalizer::CheckUnaryOperation(const UnaryExpression& expr) {
    auto operand_type = GetExpressionType(*expr.rhs);

    if (!TypeSystem::IsValidUnaryOperation(expr.operation, operand_type)) {
        ErrorReport(ErrorMsgHandler::kInvalidUnaryOperation);
        return false;
    }

    return true;
}


bool SemanticAnalizer::CheckBinaryOperation(const BinaryExpression& expr) {
    auto left_type = GetExpressionType(*expr.lhs);
    auto right_type = GetExpressionType(*expr.rhs);

    switch (expr.operation) {
        case TokenType::plus_:
            if (left_type == SemanticType::String
                || right_type == SemanticType::String)
            {
                return true;
            }

            if (left_type == SemanticType::Number
                && right_type == SemanticType::Number)
            {
                return true;
            }
            break;

            case TokenType::minus_:
                if (left_type == SemanticType::Number
                    && right_type == SemanticType::Number)
                {
                    return true;
                }
                if (left_type != SemanticType::Unknown
                    && right_type != SemanticType::Unknown)
                {
                    ErrorReport(ErrorMsgHandler::kInvalidBinaryOperation);
                    return false;
                }
                return true;

            case TokenType::star_:
            if ((left_type == SemanticType::String
                && right_type == SemanticType::Number)
                || (left_type == SemanticType::Number
                && right_type == SemanticType::String)
                || (left_type == SemanticType::String
                && right_type == SemanticType::Bool)
                || (left_type == SemanticType::Bool
                && right_type == SemanticType::String)
                || (left_type == SemanticType::Number
                && right_type == SemanticType::Number))
            {
                    return true;
                }
                if (left_type != SemanticType::Unknown
                    && right_type != SemanticType::Unknown)
                {
                    ErrorReport(ErrorMsgHandler::kInvalidBinaryOperation);
                    return false;
                }
                return true;

            case TokenType::slash_:
            case TokenType::percent_:
            case TokenType::degree_:
                if (left_type != SemanticType::Number
                    || right_type != SemanticType::Number)
                {
                    if (left_type != SemanticType::Unknown
                        && right_type != SemanticType::Unknown)
                    {
                        ErrorReport(ErrorMsgHandler::kInvalidBinaryOperation);
                        return false;
                    }
                }
                return true;

        case TokenType::double_eq_:
        case TokenType::not_eq_:
        case TokenType::less_:
        case TokenType::less_eq_:
        case TokenType::greater_:
        case TokenType::greater_eq_:
        case TokenType::and_:
        case TokenType::or_:
            return true;

        default:
            ErrorReport(ErrorMsgHandler::kInvalidBinaryOperation);
            return false;
    }
    if (left_type == SemanticType::Unknown || right_type == SemanticType::Unknown) {
        return true;
    }
    ErrorReport(ErrorMsgHandler::kInvalidBinaryOperation);
    return false;
}


bool SemanticAnalizer::CheckCallableExpression(const CallableExpression& expr) {
    if (auto var_expr = std::get_if<VariableExpression>(&expr.callable->value)) {
        if (auto builtin_info = TypeSystem::GetBuiltinInfo(var_expr->name)) {
            std::size_t arg_count = expr.f_arguments.size();
            if (arg_count < builtin_info->min_args
                || arg_count > builtin_info->max_args)
            {
                ErrorReport(ErrorMsgHandler::kWrongNumberOfArguments
                            , var_expr->name);
                return false;
            }
            if (!builtin_info->param_types.empty()) {
                for (std::size_t i = 0;
                    i < std::min(arg_count, builtin_info->param_types.size());
                    ++i)
                {
                    auto actual_type = GetExpressionType(*expr.f_arguments[i]);
                    auto expected_type = builtin_info->param_types[i];

                    if (actual_type != expected_type
                        && actual_type != SemanticType::Unknown)
                    {
                        ErrorReport(ErrorMsgHandler::kTypeMismatchInArgument
                                , var_expr->name);
                        return false;
                    }
                }
            }

            return true;
        }
    }
    auto callable_type = GetExpressionType(*expr.callable);

    if (!TypeSystem::IsCallable(callable_type)
        && callable_type != SemanticType::Unknown)
    {
        ErrorReport(ErrorMsgHandler::kAttemptingToCallNonCallableExpression);
        return false;
    }
    return true;
}


bool SemanticAnalizer::CheckIndexExpression(const IndexExpression& expr) {
    auto object_type = GetExpressionType(*expr.object);
    auto index_type = GetExpressionType(*expr.index);

    if (!TypeSystem::IsIndexable(object_type)
        && object_type != SemanticType::Unknown)
    {
        ErrorReport(ErrorMsgHandler::kAttemptingToIndexNonIndexableType);
        return false;
    }

    if (index_type != SemanticType::Number
        && index_type != SemanticType::Unknown)
    {
        ErrorReport(ErrorMsgHandler::kIndexMustBeNumber);
        return false;
    }

    return true;
}


bool SemanticAnalizer::CheckSliceExpression(const SliceExpression& expr) {
    auto object_type = GetExpressionType(*expr.object);

    if (!TypeSystem::IsSliceable(object_type)
        && object_type != SemanticType::Unknown)
    {
        ErrorReport(ErrorMsgHandler::kAttemptingToSliceNonSliceableType);
        return false;
    }

    if (expr.from_s) {
        auto from_type = GetExpressionType(*expr.from_s);
        if (from_type != SemanticType::Number
            && from_type != SemanticType::Unknown)
        {
            ErrorReport(ErrorMsgHandler::kSliceStartIndexMustBeNumber);
            return false;
        }
    }

    if (expr.to_s) {
        auto to_type = GetExpressionType(*expr.to_s);
        if (to_type != SemanticType::Number
            && to_type != SemanticType::Unknown)
        {
            ErrorReport(ErrorMsgHandler::kSliceStartIndexMustBeNumber);
            return false;
        }
    }

    return true;
}


bool SemanticAnalizer::CheckTypeCompatibility(SemanticType expected
    , SemanticType actual, const std::string& context)
{
    if (actual == SemanticType::Unknown || expected == actual) {
        return true;
    }

    if (expected == SemanticType::String && actual != SemanticType::Function) {
        return true;
    }

    ErrorReport(ErrorMsgHandler::kTypeMismatchInSlice + context);
    return false;
}
