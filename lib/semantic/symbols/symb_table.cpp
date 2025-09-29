#include <algorithm>

#include <symbols/symb_table.h>


void SymbolTable::EnterScope() {
    scopes_.emplace_back();
}


constexpr std::size_t SymbolTable::ScopeDepth() const noexcept {
    return scopes_.size();
}


constexpr bool SymbolTable::IsEmpty() const noexcept {
    return scopes_.empty();
}


void SymbolTable::ExitScope() {
    if (!scopes_.empty()) {
        scopes_.pop_back();
    }
}


bool SymbolTable::Declare(std::string_view name) {
    if (scopes_.empty()) { EnterScope(); }

    auto& current_scope = scopes_.back();
    auto name_str = std::string(name);

    if (current_scope.contains(name_str)) {
        return false;
    }

    current_scope[std::move(name_str)] = true;
    return true;
}


bool SymbolTable::Exists(std::string_view name) const noexcept {
    auto name_str = std::string(name);

    if (std::find(kBuiltinFunctions.begin(), kBuiltinFunctions.end()
        , name_str) != kBuiltinFunctions.end())
    {
        return true;
    }
    return std::any_of(scopes_.crbegin(), scopes_.crend(),
                      [&name_str](const auto& scope)
                      {
                          return scope.contains(name_str);
                      }
    );
}
