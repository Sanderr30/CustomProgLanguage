#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <random>

#include <runtime/interpreter/builtins/builtins.h>
#include <runtime/interpreter/interpreter.h>
#include <runtime/interpreter/builtins/errors/bltns_errors.h>


void BuiltinRegistry::RegisterAll(Enviroment& globals
        , std::ostream& output, std::istream& input)
{
    RegisterIOFunctions(globals, output, input);
    RegisterUtilityFunctions(globals);
    RegisterMathFunctions(globals);
    RegisterStringFunctions(globals);
    RegisterArrayFunctions(globals);
    RegisterSystemFunctions(globals);
}


void BuiltinRegistry::CheckArgumentCount(const std::vector<Value>& args
                    , std::size_t expected, const std::string& func_name)
{
    if (args.size() != expected) {
        throw BuiltinError(func_name
                        + "() expects "
                        + std::to_string(expected)
                        + " argument(s)"
        );
    }
}


void BuiltinRegistry::CheckMinArgumentCount(const std::vector<Value>& args
                        , std::size_t min_count, const std::string& func_name)
{
    if (args.size() < min_count) {
        throw BuiltinError(func_name
                        + "() expects at least "
                        + std::to_string(min_count)
                        + " argument(s)"
        );
    }
}


double BuiltinRegistry::ExtractNumber(const Value& val
                        , const std::string& func_name)
{
    if (auto* num = std::get_if<double>(&val.data)) {
        return *num;
    }
    throw BuiltinError(func_name
        + BuiltinError::kExpectedNumericArgument
    );
}


std::string BuiltinRegistry::ExtractString(const Value& val
                        , const std::string& func_name)
{
    if (auto* str = std::get_if<std::string>(&val.data)) {
        return *str;
    }
    throw BuiltinError(func_name
        + BuiltinError::kExpectedStringArgument
    );
}


Value::Array BuiltinRegistry::ExtractArray(const Value& val
                            , const std::string& func_name)
{
    if (auto* arr = std::get_if<Value::Array>(&val.data)) {
        return *arr;
    }
    throw BuiltinError(func_name
        + BuiltinError::kExpectedArrayArgument
    );
}


Value BuiltinRegistry::CreateStringArray(const
                std::vector<std::string>& strings)
{
    Value::Array result;
    result.reserve(strings.size());
    for (const auto& str : strings) {
        result.push_back(std::make_shared<Value>(str));
    }
    return Value(result);
}


void BuiltinRegistry::Register(const std::string& name
                            , BuiltinFunction func)
{
    functions_[name] = std::move(func);
}


void BuiltinRegistry::AddToEnvironment(Enviroment& env
                            , const std::string& name)
{
    auto func_obj = std::make_shared<FunctionalObject>(functions_[name]);
    env.Define(name, Value(func_obj));
}


void BuiltinRegistry::RegisterIOFunctions(Enviroment& globals
                , std::ostream& output, std::istream& input)
{
    Register("print", [&output](const std::vector<Value>& args) -> Value
    {
        if (!args.empty()) {
            const auto& v = args[0].data;
            if (auto* num = std::get_if<double>(&v)) {
                double d = *num;
                if (d == static_cast<int64_t>(d)) {
                    output << static_cast<int64_t>(d);
                } else {
                    output << d;
                }
            }
            else if (auto* str = std::get_if<std::string>(&v)) {
                if (str->find(' ') != std::string::npos) {
                    output << '"' << *str << '"';
                } else {
                    output << *str;
                }
            }
            else if (auto* boolean = std::get_if<bool>(&v)) {
                output << (*boolean ? "true" : "false");
            }
            else {
                output << "nil";
            }
        }
        return Value(NilType{});
    });
    AddToEnvironment(globals, "print");

    Register("println", [&output, this](const std::vector<Value>& args) -> Value
    {
        functions_["print"](args);
        output << '\n';
        return Value(NilType{});
    });
    AddToEnvironment(globals, "println");

    Register("read", [&input](const std::vector<Value>& args) -> Value {
        std::string line;
        if (std::getline(input, line)) {
            return Value(line);
        }
        return Value(NilType{});
    });
    AddToEnvironment(globals, "read");
}


void BuiltinRegistry::RegisterUtilityFunctions(Enviroment& globals) {
    Register("len", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "len");
        const auto& val = args[0].data;
        if (auto* str = std::get_if<std::string>(&val)) {
            return Value(static_cast<double>(str->size()));
        }
        if (auto* array = std::get_if<Value::Array>(&val)) {
            return Value(static_cast<double>(array->size()));
        }
        throw BuiltinError(BuiltinError::kArgumentMustBeStringOrArray);
    });
    AddToEnvironment(globals, "len");

    Register("type", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "type");

        const auto& val = args[0].data;
        if (std::holds_alternative<double>(val)) { return Value("number"); }
        if (std::holds_alternative<std::string>(val)) { return Value("string"); }
        if (std::holds_alternative<bool>(val)) { return Value("boolean"); }
        if (std::holds_alternative<NilType>(val)) { return Value("nil"); }
        if (std::holds_alternative<Value::Array>(val)) { return Value("array"); }
        if (std::holds_alternative<Value::FuncPtr>(val)) { return Value("function"); }
        return Value("unknown");
    });
    AddToEnvironment(globals, "type");
}


void BuiltinRegistry::RegisterMathFunctions(Enviroment& globals) {
    Register("abs", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "abs");
        double val = ExtractNumber(args[0], "abs");
        return Value(std::abs(val));
    });
    AddToEnvironment(globals, "abs");

    Register("ceil", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "ceil");
        double val = ExtractNumber(args[0], "ceil");
        return Value(std::ceil(val));
    });
    AddToEnvironment(globals, "ceil");

    Register("floor", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "floor");
        double val = ExtractNumber(args[0], "floor");
        return Value(std::floor(val));
    });
    AddToEnvironment(globals, "floor");

    Register("round", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "round");
        double val = ExtractNumber(args[0], "round");
        return Value(std::round(val));
    });
    AddToEnvironment(globals, "round");

    Register("rnd", [this](const std::vector<Value>& args) -> Value {
        CheckArgumentCount(args, 1, "rnd");
        int n = static_cast<int>(ExtractNumber(args[0], "rnd"));
        if (n <= 0) {
            throw BuiltinError(BuiltinError::kRndArgumentMustBePositive);
        }

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, n - 1);
        return Value(static_cast<double>(dis(gen)));
    });

    Register("sqrt", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "sqrt");
        double val = ExtractNumber(args[0], "sqrt");
        if (val < 0) {
            throw BuiltinError(BuiltinError::kSqrtOfNegativeNumber);
        }
        return Value(std::sqrt(val));
    });
    AddToEnvironment(globals, "sqrt");

    Register("parse_num", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "parse_num");
        const std::string& str = ExtractString(args[0], "parse_num");
        try {
            double val = std::stod(str);
            return Value(val);
        } catch (...) {
            return Value(NilType{});
        }
    });
    AddToEnvironment(globals, "parse_num");

    Register("to_string", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "to_string");
        double val = ExtractNumber(args[0], "to_string");
        if (val == static_cast<int64_t>(val)) {
            return Value(std::to_string(static_cast<int64_t>(val)));
        }
        return Value(std::to_string(val));
    });
    AddToEnvironment(globals, "to_string");

    Register("min", [this](const std::vector<Value>& args) -> Value
    {
        CheckMinArgumentCount(args, 2, "min");
        double min_val = ExtractNumber(args[0], "min");
        for (std::size_t i = 1; i < args.size(); ++i) {
            min_val = std::min(min_val, ExtractNumber(args[i], "min"));
        }
        return Value(min_val);
    });
    AddToEnvironment(globals, "min");

    Register("max", [this](const std::vector<Value>& args) -> Value
    {
        CheckMinArgumentCount(args, 2, "max");
        double max_val = ExtractNumber(args[0], "max");
        for (std::size_t i = 1; i < args.size(); ++i) {
            max_val = std::max(max_val, ExtractNumber(args[i], "max"));
        }
        return Value(max_val);
    });
    AddToEnvironment(globals, "max");
}


void BuiltinRegistry::RegisterStringFunctions(Enviroment& globals) {
    Register("lower", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "lower");
        std::string str = ExtractString(args[0], "lower");
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return Value(str);
    });
    AddToEnvironment(globals, "lower");

    Register("upper", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "upper");
        std::string str = ExtractString(args[0], "upper");
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return Value(str);
    });
    AddToEnvironment(globals, "upper");

    Register("split", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 2, "split");
        const std::string& str = ExtractString(args[0], "split");
        const std::string& delim = ExtractString(args[1], "split");

        std::vector<std::string> result;
        if (delim.empty()) {
            throw BuiltinError(BuiltinError::kSplitDelimiterCannotBeEmpty);
        }

        std::size_t start = 0;
        std::size_t end = str.find(delim);

        while (end != std::string::npos) {
            result.push_back(str.substr(start, end - start));
            start = end + delim.length();
            end = str.find(delim, start);
        }
        result.push_back(str.substr(start));

        return CreateStringArray(result);
    });
    AddToEnvironment(globals, "split");

    Register("join", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 2, "join");
        Value::Array array = ExtractArray(args[0], "join");
        const std::string& delim = ExtractString(args[1], "join");

        std::ostringstream oss;
        for (std::size_t i = 0; i < array.size(); ++i) {
            if (i > 0) oss << delim;

            const auto& v = array[i]->data;
            if (auto* str = std::get_if<std::string>(&v)) {
                oss << *str;
            } else if (auto* num = std::get_if<double>(&v)) {
                oss << *num;
            } else if (auto* boolean = std::get_if<bool>(&v)) {
                oss << (*boolean ? "true" : "false");
            } else {
                oss << "nil";
            }
        }

        return Value(oss.str());
    });
    AddToEnvironment(globals, "join");

    Register("replace", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 3, "replace");
        std::string str = ExtractString(args[0], "replace");
        const std::string& old_str = ExtractString(args[1], "replace");
        const std::string& new_str = ExtractString(args[2], "replace");

        if (old_str.empty()) {
            throw BuiltinError(BuiltinError::kReplaceOldStringCannotBeEmpty);
        }

        std::size_t pos = 0;
        while ((pos = str.find(old_str, pos)) != std::string::npos) {
            str.replace(pos, old_str.length(), new_str);
            pos += new_str.length();
        }

        return Value(str);
    });
    AddToEnvironment(globals, "replace");
}


void BuiltinRegistry::RegisterArrayFunctions(Enviroment& globals) {
    Register("range", [this](const std::vector<Value>& args) -> Value
        {
        if (args.empty() || args.size() > 3) {
            throw BuiltinError(BuiltinError::kRangeInvalidArguments);
        }

        double start = 0;
        double end = 0;
        double step = 1;

        if (args.size() == 1) {
            end = ExtractNumber(args[0], "range");
        } else if (args.size() == 2) {
            start = ExtractNumber(args[0], "range");
            end = ExtractNumber(args[1], "range");
        } else {
            start = ExtractNumber(args[0], "range");
            end = ExtractNumber(args[1], "range");
            step = ExtractNumber(args[2], "range");
        }

        if (step == 0) {
            throw BuiltinError(BuiltinError::kRangeStepCannotBeZero);
        }

        Value::Array result;
        if (step > 0) {
            for (double v = start; v < end; v += step) {
                result.push_back(std::make_shared<Value>(v));
            }
        } else {
            for (double v = start; v > end; v += step) {
                result.push_back(std::make_shared<Value>(v));
            }
        }
        return Value(result);
    });
    AddToEnvironment(globals, "range");

    Register("push", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 2, "push");
        Value::Array array = ExtractArray(args[0], "push");
        array.push_back(std::make_shared<Value>(args[1]));
        return Value(array);
    });
    AddToEnvironment(globals, "push");

    Register("pop", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "pop");
        Value::Array array = ExtractArray(args[0], "pop");
        if (array.empty()) {
            throw BuiltinError(BuiltinError::kPopFromEmptyArray);
        }
        Value result = *array.back();
        array.pop_back();
        return result;
    });
    AddToEnvironment(globals, "pop");

    Register("insert", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 3, "insert");
        Value::Array array = ExtractArray(args[0], "insert");
        int index = static_cast<int>(ExtractNumber(args[1], "insert"));

        if (index < 0) {
            index += static_cast<int>(array.size());
        }
        if (index < 0 || index > static_cast<int>(array.size())) {
            throw BuiltinError(BuiltinError::kInsertIndexOutOfRange);
        }

        array.insert(array.begin() + index, std::make_shared<Value>(args[2]));
        return Value(array);
    });

    AddToEnvironment(globals, "insert");

    Register("remove", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 2, "remove");
        Value::Array array = ExtractArray(args[0], "remove");
        int index = static_cast<int>(ExtractNumber(args[1], "remove"));

        if (index < 0)  {
            index += static_cast<int>(array.size());
        }
        if (index < 0 || index >= static_cast<int>(array.size())) {
            throw BuiltinError(BuiltinError::kRemoveIndexOutOfRange);
        }

        Value result = *array[index];
        array.erase(array.begin() + index);
        return result;
    });

    AddToEnvironment(globals, "remove");

    Register("sort", [this](const std::vector<Value>& args) -> Value
    {
        CheckArgumentCount(args, 1, "sort");
        Value::Array array = ExtractArray(args[0], "sort");

        std::sort(array.begin(), array.end(), [](const auto& a, const auto& b) {
            if (std::holds_alternative<double>(a->data)
                && std::holds_alternative<double>(b->data))
            {
                return (std::get<double>(a->data) < std::get<double>(b->data));
            }

            if (std::holds_alternative<std::string>(a->data)
                && std::holds_alternative<std::string>(b->data))
            {
                return (std::get<std::string>(a->data) < std::get<std::string>(b->data));
            }
            return false;
        });

        return Value(array);
    });

    AddToEnvironment(globals, "sort");
}


void BuiltinRegistry::RegisterSystemFunctions(Enviroment& globals) {
    Register("stacktrace", [](const std::vector<Value>& args) -> Value
    {
        return Value(Interpreter::GetStackTrace());
    });
    AddToEnvironment(globals, "stacktrace");
}
