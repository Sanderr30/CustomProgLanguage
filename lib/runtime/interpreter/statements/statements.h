#pragma once
#include <runtime/value/value.h>
#include <runtime/enviroment/enviroment.h>
#include <vls_and_sttmnts.h>
#include <semantic.h>

class Interpreter;

class StatementProcessor {
public:
    StatementProcessor(Interpreter* interpreter) : interpreter_(interpreter) {}

    template<StatementLike T>
    void Process(const T&, Enviroment*);


    void ProcessExpression(const ExpressionStatement&, Enviroment*);
    void ProcessIf(const IfStatement&, Enviroment*);
    void ProcessWhile(const WhileStatement&, Enviroment*);
    void ProcessFor(const ForStatement&, Enviroment*);
    void ProcessReturn(const ReturnStatement&, Enviroment*);
    void ProcessBlock(const BlockStatement&, Enviroment*);
    void ProcessBreak(const BreakStatement&, Enviroment*);
    void ProcessContinue(const ContinueStatement&, Enviroment*);

private:
    Interpreter* interpreter_;
};
