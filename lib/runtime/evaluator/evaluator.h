#pragma once


#include <runtime/interpreter/interpreter.h>
#include <runtime/evaluator/errors/ev_errors.h>
#include <runtime/evaluator/operations/handlers.h>
#include <runtime/evaluator/operations/register.h>
#include <semantic.h>

class Interpreter;


class ExpressionEvaluator {
public:
    ExpressionEvaluator(Interpreter* interpreter, Enviroment* env);

    Value operator()(const NumberExpression&) const;
    Value operator()(const StringExpression&) const;
    Value operator()(const BoolExpression&) const;
    Value operator()(const NilExpression&) const;
    Value operator()(const VariableExpression&) const;
    Value operator()(const UnaryExpression&) const;
    Value operator()(const BinaryExpression&) const;
    Value operator()(const CallableExpression&) const;
    Value operator()(const ListExpression&) const;
    Value operator()(const FunctionExpression&) const;
    Value operator()(const AssignExpression&) const;
    Value operator()(const IndexExpression&) const;
    Value operator()(const SliceExpression&) const;

private:
    Interpreter* interpreter_;
    Enviroment* env_;
};
