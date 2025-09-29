#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <stack>

#include <runtime/value/value.h>
#include <runtime/function/function.h>
#include <runtime/enviroment/enviroment.h>
#include <vls_and_sttmnts.h>
#include <semantic.h>
#include <syntax.h>
#include <runtime/evaluator/evaluator.h>
#include <runtime/interpreter/interpreter.h>
#include <runtime/interpreter/statements/statements.h>
#include <runtime/interpreter/builtins/builtins.h>



class ExpressionEvaluator;

class StatementProcessor;


struct CallFrame {
    std::string function_name;
    std::vector<std::string> parameters;
    std::vector<Value> arguments;

    CallFrame(const std::string& name,
              const std::vector<std::string>& params = {},
              const std::vector<Value>& args = {})
        : function_name(name)
        , parameters(params)
        , arguments(args)
    {}
};


class Interpreter {
public:
    static bool Interpret(std::istream&, std::ostream&);

    Value ParseNode(const Expression&, Enviroment*);
    void Perform(const Statement&, Enviroment*);
    Value ParseList(const std::vector<Statement>&, Enviroment*);
    Value PerformFunction(const Value::FuncPtr&, const std::vector<Value>&);

    bool IsTrue(const Value&) const;
    bool IsEqual(const Value&, const Value&) const;
    Enviroment& GetGlobals();

    static void PushCallFrame(const CallFrame& frame);
    static void PopCallFrame();
    static std::string GetStackTrace();

private:
    Enviroment globals_;
    std::ostream& output_;
    std::unique_ptr<StatementProcessor> statement_processor_;

    static std::stack<CallFrame> call_stack_;

    Interpreter(std::ostream&);
    void RegisterBuiltins();

    friend class ExpressionEvaluator;
};

bool RunInterpreter(std::istream& in, std::ostream& out);
