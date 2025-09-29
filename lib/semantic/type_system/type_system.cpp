#include <algorithm>
#include <climits>

#include <type_system/type_system.h>


const std::vector<BuiltinFunctionInfo> TypeSystem::builtin_functions_ = {
    {"print", {}, SemanticType::Nil, 0, SIZE_MAX}
    , {"println", {}, SemanticType::Nil, 0, SIZE_MAX}
    , {"read", {}, SemanticType::String, 0, 0}
    , {"abs", {SemanticType::Number}, SemanticType::Number, 1, 1}
    , {"ceil", {SemanticType::Number}, SemanticType::Number, 1, 1}
    , {"floor", {SemanticType::Number}, SemanticType::Number, 1, 1}
    , {"round", {SemanticType::Number}, SemanticType::Number, 1, 1}
    , {"sqrt", {SemanticType::Number}, SemanticType::Number, 1, 1}
    , {"rnd", {}, SemanticType::Number, 0, 0}
    , {"parse_num", {SemanticType::String}, SemanticType::Number, 1, 1}
    , {"to_string", {}, SemanticType::String, 1, 1}
    , {"len", {}, SemanticType::Number, 1, 1}
    , {"lower", {SemanticType::String}, SemanticType::String, 1, 1}
    , {"upper", {SemanticType::String}, SemanticType::String, 1, 1}
    , {"split", {SemanticType::String, SemanticType::String}, SemanticType::List, 2, 2}
    , {"join", {SemanticType::List, SemanticType::String}, SemanticType::String, 2, 2}
    , {"replace", {SemanticType::String, SemanticType::String, SemanticType::String}, SemanticType::String, 3, 3}
    , {"range", {SemanticType::Number}, SemanticType::List, 1, 3}
    , {"push", {SemanticType::List}, SemanticType::Nil, 2, 2}
    , {"pop", {SemanticType::List}, SemanticType::Unknown, 1, 1}
    , {"insert", {SemanticType::List, SemanticType::Number}, SemanticType::Nil, 3, 3}
    , {"remove", {SemanticType::List, SemanticType::Number}, SemanticType::Unknown, 2, 2}
    , {"sort", {SemanticType::List}, SemanticType::Nil, 1, 1}
    , {"stacktrace", {}, SemanticType::Nil, 0, 0}
};


bool TypeSystem::IsValidUnaryOperation(TokenType op, SemanticType operand) {
    switch (op) {
        case TokenType::minus_:
            return operand == SemanticType::Number;
        case TokenType::not_:
            return true;
        default:
            return false;
    }
}


bool TypeSystem::IsIndexable(SemanticType type) {
    return (type == SemanticType::List
    || type == SemanticType::String);
}


bool TypeSystem::IsSliceable(SemanticType type) {
    return (type == SemanticType::List
    || type == SemanticType::String
    || type == SemanticType::Function);
}


bool TypeSystem::IsCallable(SemanticType type) {
    return type == SemanticType::Function;
}


SemanticType TypeSystem::GetBinaryResultType(TokenType op
                , SemanticType left, SemanticType right)
{
    switch (op) {
        case TokenType::plus_:
            if (left == SemanticType::String
                || right == SemanticType::String)
            {
                return SemanticType::String;
            }
            return SemanticType::Number;

        case TokenType::star_:
            if (left == SemanticType::String || right == SemanticType::String) {
                return SemanticType::String;
            }
            return SemanticType::Number;
        case TokenType::minus_:
        case TokenType::slash_:
        case TokenType::percent_:
        case TokenType::degree_:
            return SemanticType::Number;

        case TokenType::double_eq_:
        case TokenType::not_eq_:
        case TokenType::less_:
        case TokenType::less_eq_:
        case TokenType::greater_:
        case TokenType::greater_eq_:
        case TokenType::and_:
        case TokenType::or_:
            return SemanticType::Bool;

        default:
            return SemanticType::Unknown;
    }
}


SemanticType TypeSystem::GetUnaryResultType(TokenType op, SemanticType operand) {
    if (!IsValidUnaryOperation(op, operand)) {
        return SemanticType::Unknown;
    }

    switch (op) {
        case TokenType::minus_:
            return SemanticType::Number;
        case TokenType::not_:
            return SemanticType::Bool;
        default:
            return SemanticType::Unknown;
    }
}


const BuiltinFunctionInfo* TypeSystem::GetBuiltinInfo(const std::string& name) {
    auto it = std::find_if(builtin_functions_.begin(), builtin_functions_.end(),
                          [&name](const BuiltinFunctionInfo& info)
                          {
                              return info.name == name;
                          }
    );
    return it != builtin_functions_.end() ? &(*it) : nullptr;
}


const std::vector<BuiltinFunctionInfo>& TypeSystem::GetBuiltinFunctions() {
    return builtin_functions_;
}


bool TypeSystem::IsBuiltinFunction(const std::string& name) {
    return GetBuiltinInfo(name) != nullptr;
}
