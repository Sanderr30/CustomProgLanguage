#include <iostream>
#include <sstream>

#include <runtime/interpreter/errors/intrptr_errors.h>
#include <semantic.h>
#include <syntax.h>
#include <runtime/interpreter/interpreter.h>

std::stack<CallFrame> Interpreter::call_stack_;


bool RunInterpreter(std::istream& in, std::ostream& out) {
    return Interpreter::Interpret(in, out);
}


Enviroment& Interpreter::GetGlobals() {
    return globals_;
}


bool Interpreter::Interpret(std::istream& in, std::ostream& out) {
    try {
        SyntaxAnalizer parser(in);
        auto program = parser.Parse();
        SemanticAnalizer sem(out);
        if (!sem.Analyse(program)) { return false; }

        Interpreter interp(out);
        interp.RegisterBuiltins();
        PushCallFrame(CallFrame("[global]"));
        interp.ParseList(program, &interp.globals_);
        PopCallFrame();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Interpreter error: " << e.what() << "\n";
        return false;
    } catch (...) {
        std::cerr << InterpreterError::kUnknownError;
        return false;
    }
}


Interpreter::Interpreter(std::ostream& out)
    : globals_()
    , output_(out)
    , statement_processor_(std::make_unique<StatementProcessor>(this))
{}


Value Interpreter::ParseNode(const Expression& expr, Enviroment* env) {
    return std::visit(ExpressionEvaluator{this, env}, expr.value);
}


void Interpreter::Perform(const Statement& stmt, Enviroment* env) {
    std::visit([&](const auto& s) {
        statement_processor_->Process(s, env);
    }, stmt.value);
}


Value Interpreter::ParseList(const std::vector<Statement>& stmts, Enviroment* parent) {
    Enviroment block(parent);
    for (const auto& stmt : stmts) {
        Perform(stmt, &block);
    }
    return Value(NilType{});
}


Value Interpreter::PerformFunction(const Value::FuncPtr& fn, const std::vector<Value>& args) {
    if (fn->native) {
        return fn->native(args);
    }

    std::string func_name = "[anonymous]";
    if (!fn->parameters.empty()) {
        func_name = "[function(" + std::to_string(fn->parameters.size()) + " params)]";
    }

    Enviroment local(fn->closure);
    for (std::size_t i = 0; i < fn->parameters.size(); ++i) {
        Value v = (i < args.size()) ? args[i] : Value(NilType{});
        local.Define(fn->parameters[i], v);
    }

    try {
        for (const auto& stmt : *fn->f_body) {
            Perform(stmt, &local);
        }
    } catch (const ReturnException& r) {
        return r.value;
    }

    PopCallFrame();

    return Value(NilType{});
}


bool Interpreter::IsTrue(const Value& v) const {
    if (auto* boolean = std::get_if<bool>(&v.data)) {
        return *boolean;
    }
    if (std::holds_alternative<NilType>(v.data)) {
        return false;
    }
    return true;
}


void Interpreter::PushCallFrame(const CallFrame& frame) {
    call_stack_.push(frame);
}


void Interpreter::PopCallFrame() {
    if (!call_stack_.empty()) {
        call_stack_.pop();
    }
}


std::string Interpreter::GetStackTrace() {
    if (call_stack_.empty()) {
        return "stacktrace: [empty stack]";
    }

    std::ostringstream oss;
    oss << "stacktrace: ";

    std::stack<CallFrame> temp_stack = call_stack_;
    std::vector<CallFrame> frames;

    while (!temp_stack.empty()) {
        frames.push_back(temp_stack.top());
        temp_stack.pop();
    }

    for (int i = frames.size() - 1; i >= 0; --i) {
        oss << frames[i].function_name;
        if (i > 0) {
            oss << " -> ";
        }
    }

    return oss.str();
}


bool Interpreter::IsEqual(const Value& a, const Value& b) const {
    if (a.data.index() != b.data.index()) {
        return false;
    }

    if (auto* num_a = std::get_if<double>(&a.data)) {
        return *num_a == std::get<double>(b.data);
    }
    if (auto* str_a = std::get_if<std::string>(&a.data)) {
        return *str_a == std::get<std::string>(b.data);
    }
    if (auto* bool_a = std::get_if<bool>(&a.data)) {
        return *bool_a == std::get<bool>(b.data);
    }
    if (std::holds_alternative<NilType>(a.data)) {
        return true;
    }

    return false;
}


void Interpreter::RegisterBuiltins() {
    BuiltinRegistry::Get().RegisterAll(globals_, output_, std::cin);
}
