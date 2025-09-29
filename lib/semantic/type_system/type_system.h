#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <syntax.h>


enum class SemanticType {
    Number, String
    , Bool, Nil
    , List, Function
    , Unknown
};


struct BuiltinFunctionInfo {
    std::string name;
    std::vector<SemanticType> param_types;
    SemanticType return_type;
    std::size_t min_args;
    std::size_t max_args;
};


class TypeSystem {
public:
    static const std::vector<BuiltinFunctionInfo>& GetBuiltinFunctions();

    static bool IsBuiltinFunction(const std::string&);

    static const BuiltinFunctionInfo* GetBuiltinInfo(const std::string&);

    static bool IsValidUnaryOperation(TokenType, SemanticType);

    static bool IsIndexable(SemanticType);
    static bool IsSliceable(SemanticType);
    static bool IsCallable(SemanticType);

    static SemanticType GetBinaryResultType(TokenType, SemanticType, SemanticType);
    static SemanticType GetUnaryResultType(TokenType, SemanticType);

private:
    static const std::vector<BuiltinFunctionInfo> builtin_functions_;
    static const std::unordered_map<std::string, std::size_t> builtin_lookup_;
};
