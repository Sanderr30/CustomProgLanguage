#pragma once

#include <string>
#include <vector>
#include <memory>
#include <variant>

#include <token/token.h>

struct Expression;

struct Statement;

struct NilExpression { };

struct BreakStatement { };

struct ContinueStatement { };


struct NumberExpression {
    NumberExpression(double);
    double value;
};

struct StringExpression {
    StringExpression(const std::string&);
    std::string value;
};

struct BoolExpression {
    BoolExpression(bool);
    bool value;
};

struct VariableExpression {
    VariableExpression(const std::string&);
    std::string name;
};

struct UnaryExpression {
    TokenType operation;
    std::unique_ptr<Expression> rhs;
};

struct BinaryExpression {
    std::unique_ptr<Expression> lhs;
    TokenType operation;
    std::unique_ptr<Expression> rhs;
};

struct CallableExpression {
    std::unique_ptr<Expression> callable;
    std::vector<std::unique_ptr<Expression>> f_arguments;
};

struct ListExpression {
    std::vector<std::unique_ptr<Expression>> elements;
};

struct FunctionExpression {
    std::vector<std::string> parameters;
    std::vector<Statement> f_body;
};

struct AssignExpression {
    std::string name;
    TokenType operation;
    std::unique_ptr<Expression> rhs;
};

struct IndexExpression {
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> index;
};

struct SliceExpression {
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> from_s;
    std::unique_ptr<Expression> to_s;
};

using ExpressionVariant = std::variant
<
    NumberExpression, StringExpression
    , BoolExpression, NilExpression
    , VariableExpression, UnaryExpression
    , BinaryExpression, CallableExpression
    , ListExpression, FunctionExpression
    , AssignExpression, IndexExpression
    , SliceExpression
>;

struct Expression {
    ExpressionVariant value;

    template<typename T>
    Expression(T&& v)
        : value(std::forward<T>(v))
    {}
};

struct ExpressionStatement {
    Expression expression;
};

struct IfStatement {
    Expression condition;
    std::vector<Statement> then_case;
    std::vector<Statement> else_case;
};

struct WhileStatement {
    Expression condition;
    std::vector<Statement> body;
};

struct ForStatement {
    std::string var;
    Expression iter;
    std::vector<Statement> body;
};

struct ReturnStatement {
    std::unique_ptr<Expression> value;
};

struct BlockStatement {
    std::vector<Statement> statements;
};

using StatementVariant = std::variant
<
    ExpressionStatement, IfStatement
    , WhileStatement, ForStatement
    , ReturnStatement, BlockStatement
    , BreakStatement, ContinueStatement
>;

struct Statement {
    template<typename T>
    Statement(T&& v)
        : value(std::forward<T>(v))
    {}

    StatementVariant value;
};
