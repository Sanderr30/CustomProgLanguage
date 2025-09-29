#pragma once

#include <vector>
#include <ostream>
#include <algorithm>

#include <syntax.h>
#include <symbols/symb_table.h>
#include <errors/semantic_errors.h>
#include <type_system/type_system.h>


template<typename T>
concept StatementLike = requires {
    requires
    std::is_same_v<T, ExpressionStatement>
    || std::is_same_v<T, IfStatement>
    || std::is_same_v<T, WhileStatement>
    || std::is_same_v<T, ForStatement>
    || std::is_same_v<T, ReturnStatement>
    || std::is_same_v<T, BlockStatement>
    || std::is_same_v<T, BreakStatement>
    || std::is_same_v<T, ContinueStatement>;
};


template<typename T>
concept ExpressionLike = requires {
    requires
    std::is_same_v<T, NumberExpression>
    || std::is_same_v<T, StringExpression>
    || std::is_same_v<T, BoolExpression>
    || std::is_same_v<T, NilExpression>
    || std::is_same_v<T, VariableExpression>
    || std::is_same_v<T, UnaryExpression>
    || std::is_same_v<T, BinaryExpression>
    || std::is_same_v<T, CallableExpression>
    || std::is_same_v<T, ListExpression>
    || std::is_same_v<T, FunctionExpression>
    || std::is_same_v<T, AssignExpression>
    || std::is_same_v<T, IndexExpression>
    || std::is_same_v<T, SliceExpression>;
};


class SemanticAnalizer {
public:
    SemanticAnalizer(std::ostream&);
    bool Analyse(const std::vector<Statement>&);

private:
    SemanticType GetExpressionType(const Expression&);
    bool CheckTypeCompatibility(SemanticType, SemanticType, const std::string&);
    bool CheckBinaryOperation(const BinaryExpression&);
    bool CheckUnaryOperation(const UnaryExpression&);
    bool CheckCallableExpression(const CallableExpression&);
    bool CheckIndexExpression(const IndexExpression&);
    bool CheckSliceExpression(const SliceExpression&);

private:
    bool ProcessStatement(const Statement&);

    bool ProcessExpression(const Expression&);

    void ErrorReport(const std::string&, const std::string& details = "");

    template<StatementLike T>
    bool ProcessStatementImpl(const T&);

    template<ExpressionLike T>
    bool ProcessExpressionImpl(const T&);

    template<typename Container>
    bool ProcessStatements(const Container&);

    template<typename Container>
    bool ProcessExpressions(const Container&);

private:
    std::ostream& error_stream_;
    SymbolTable symbol_table_;
    std::unordered_map<std::string, SemanticType> variable_types_;
};


template<>
inline constexpr bool SemanticAnalizer::ProcessExpressionImpl(const NumberExpression&) {
    return true;
}

template<>
inline constexpr bool SemanticAnalizer::ProcessExpressionImpl(const StringExpression&) {
    return true;
}

template<>
inline constexpr bool SemanticAnalizer::ProcessExpressionImpl(const BoolExpression&) {
    return true;
}

template<>
inline constexpr bool SemanticAnalizer::ProcessExpressionImpl(const NilExpression&) {
    return true;
}

template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const BreakStatement&) {
    return true;
}

template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const ContinueStatement&) {
    return true;
}

template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const ExpressionStatement& stmt) {
    return ProcessExpression(stmt.expression);
}


template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const IfStatement& stmt) {
    bool success = ProcessExpression(stmt.condition);

    symbol_table_.EnterScope();
    success &= ProcessStatements(stmt.then_case);
    symbol_table_.ExitScope();

    if (!stmt.else_case.empty()) {
        symbol_table_.EnterScope();
        success &= ProcessStatements(stmt.else_case);
        symbol_table_.ExitScope();
    }

    return success;
}


template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const WhileStatement& stmt) {
    bool success = ProcessExpression(stmt.condition);

    symbol_table_.EnterScope();
    success &= ProcessStatements(stmt.body);
    symbol_table_.ExitScope();

    return success;
}


template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const ForStatement& stmt) {
    bool success = ProcessExpression(stmt.iter);
    symbol_table_.EnterScope();
    if (!symbol_table_.Declare(stmt.var)) {
        ErrorReport(ErrorMsgHandler::kVariableAlreadyDeclared, stmt.var);
        return false;
    }
    success &= ProcessStatements(stmt.body);
    symbol_table_.ExitScope();
    return success;
}


template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const ReturnStatement& stmt) {
    return stmt.value ? ProcessExpression(*stmt.value) : true;
}


template<>
inline bool SemanticAnalizer::ProcessStatementImpl(const BlockStatement& stmt) {
    symbol_table_.EnterScope();
    bool success = ProcessStatements(stmt.statements);
    symbol_table_.ExitScope();
    return success;
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const VariableExpression& expr) {
    if (!symbol_table_.Exists(expr.name)) {
        ErrorReport(ErrorMsgHandler::kUndefinedVariable, expr.name);
        return false;
    }
    return true;
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const UnaryExpression& expr) {
    bool success = ProcessExpression(*expr.rhs);
    if (success) {
        success &= CheckUnaryOperation(expr);
    }
    return success;
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const BinaryExpression& expr) {
    bool success = ProcessExpression(*expr.lhs);
    success &= ProcessExpression(*expr.rhs);
    if (success) { success &= CheckBinaryOperation(expr); }
    return success;
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const CallableExpression& expr) {
    bool success = ProcessExpression(*expr.callable);
    success &= ProcessExpressions(expr.f_arguments);
    if (success) { success &= CheckCallableExpression(expr); }
    return success;
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const ListExpression& expr) {
    return ProcessExpressions(expr.elements);
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const FunctionExpression& expr) {
    symbol_table_.EnterScope();
    bool success = true;
    for (const auto& param : expr.parameters) {
        if (!symbol_table_.Declare(param)) {
            ErrorReport(ErrorMsgHandler::kDuplicatedParameter, param);
            success = false;
        }
    }
    success &= ProcessStatements(expr.f_body);
    symbol_table_.ExitScope();
    return success;
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const AssignExpression& expr) {
    if (!symbol_table_.Exists(expr.name)) {
        if (!symbol_table_.Declare(expr.name)) {
            ErrorReport(ErrorMsgHandler::kFailingDeclaration, expr.name);
            return false;
        }
    }
    bool success = ProcessExpression(*expr.rhs);
    if (success) {
        auto rhs_type = GetExpressionType(*expr.rhs);
        variable_types_[expr.name] = rhs_type;
    }
    return ProcessExpression(*expr.rhs);
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const IndexExpression& expr) {
    bool success = ProcessExpression(*expr.object);
    success &= ProcessExpression(*expr.index);
    if (success) { success &= CheckIndexExpression(expr); }
    return success;
}


template<>
inline bool SemanticAnalizer::ProcessExpressionImpl(const SliceExpression& expr) {
    bool success = ProcessExpression(*expr.object);
    if (expr.from_s) { success &= ProcessExpression(*expr.from_s); }
    if (expr.to_s) { success &= ProcessExpression(*expr.to_s); }
    if (success) { success &= CheckSliceExpression(expr); }
    return success;
}


template<typename Container>
inline bool SemanticAnalizer::ProcessStatements(const Container& statements) {
    return std::all_of(statements.begin(), statements.end(),
                      [this](const auto& stmt)
                      {
                          return ProcessStatement(stmt);
                      }
    );
}


template<typename Container>
inline bool SemanticAnalizer::ProcessExpressions(const Container& expressions) {
    return std::all_of(expressions.begin(), expressions.end(),
                      [this](const auto& expr)
                      {
                          return ProcessExpression(*expr);
                      }
    );
}
