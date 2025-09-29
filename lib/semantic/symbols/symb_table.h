#pragma once

#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>


class SymbolTable {
public:
    void EnterScope();
    void ExitScope();

    bool Declare(std::string_view);
    bool Exists(std::string_view) const noexcept;

    constexpr std::size_t ScopeDepth() const noexcept;
    constexpr bool IsEmpty() const noexcept;

private:
    std::vector<std::unordered_map<std::string, bool>> scopes_;

    static constexpr std::array kBuiltinFunctions =
    {
        "print", "println",
        "read", "stacktrace",
        "abs", "ceil",
        "floor", "round",
        "sqrt", "rnd",
        "parse_num", "to_string",
        "min", "max",
        "len", "lower",
        "upper", "split",
        "join", "replace",
        "range", "push",
        "pop", "insert",
        "remove", "sort",
        "type"
    };


    friend class SemanticAnalizer;
};
