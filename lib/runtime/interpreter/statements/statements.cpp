#include <runtime/interpreter/errors/intrptr_errors.h>
#include <runtime/interpreter/statements/statements.h>
#include <runtime/function/errors/func_errors.h>
#include <runtime/interpreter/interpreter.h>


template<>
void StatementProcessor::Process(const ExpressionStatement& stmt, Enviroment* env) {
    ProcessExpression(stmt, env);
}


template<>
void StatementProcessor::Process(const IfStatement& stmt, Enviroment* env) {
    ProcessIf(stmt, env);
}


template<>
void StatementProcessor::Process(const WhileStatement& stmt, Enviroment* env) {
    ProcessWhile(stmt, env);
}


template<>
void StatementProcessor::Process(const ForStatement& stmt, Enviroment* env) {
    ProcessFor(stmt, env);
}


template<>
void StatementProcessor::Process(const ReturnStatement& stmt, Enviroment* env) {
    ProcessReturn(stmt, env);
}


template<>
void StatementProcessor::Process(const BlockStatement& stmt, Enviroment* env) {
    ProcessBlock(stmt, env);
}


template<>
void StatementProcessor::Process(const BreakStatement& stmt, Enviroment* env) {
    ProcessBreak(stmt, env);
}


template<>
void StatementProcessor::Process(const ContinueStatement& stmt, Enviroment* env) {
    ProcessContinue(stmt, env);
}


void StatementProcessor::ProcessExpression(const ExpressionStatement& stmt, Enviroment* env) {
    interpreter_->ParseNode(stmt.expression, env);
}


void StatementProcessor::ProcessIf(const IfStatement& stmt, Enviroment* env) {
    if (interpreter_->IsTrue(interpreter_->ParseNode(stmt.condition, env))) {
        interpreter_->ParseList(stmt.then_case, env);
    } else if (!stmt.else_case.empty()) {
        interpreter_->ParseList(stmt.else_case, env);
    }
}


void StatementProcessor::ProcessWhile(const WhileStatement& stmt, Enviroment* env) {
    try {
        while (interpreter_->IsTrue(interpreter_->ParseNode(stmt.condition, env))) {
            try {
                interpreter_->ParseList(stmt.body, env);
            } catch (const ContinueException&) {
                continue;
            }
        }
    } catch (const BreakException&) { }
}


void StatementProcessor::ProcessFor(const ForStatement& stmt, Enviroment* env) {
    Value iterable = interpreter_->ParseNode(stmt.iter, env);
    if (!std::holds_alternative<Value::Array>(iterable.data)) {
        throw InterpreterError(InterpreterError::kCanOnlyIterateArrays);
    }

    auto& array = std::get<Value::Array>(iterable.data);
    try {
        for (const auto& element : array) {
            Enviroment loop_env(env);
            loop_env.Define(stmt.var, *element);
            try {
                interpreter_->ParseList(stmt.body, &loop_env);
            } catch (const ContinueException&) {
                continue;
            }
        }
    } catch (const BreakException&) { }
}


void StatementProcessor::ProcessReturn(const ReturnStatement& stmt, Enviroment* env) {
    if (stmt.value) {
        throw ReturnException(interpreter_->ParseNode(*stmt.value, env));
    }
    throw ReturnException(Value(NilType{}));
}


void StatementProcessor::ProcessBlock(const BlockStatement& stmt, Enviroment* env) {
    interpreter_->ParseList(stmt.statements, env);
}


void StatementProcessor::ProcessBreak(const BreakStatement&, Enviroment*) {
    throw BreakException();
}


void StatementProcessor::ProcessContinue(const ContinueStatement&, Enviroment*) {
    throw ContinueException();
}
