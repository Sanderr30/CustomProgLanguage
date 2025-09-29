#include "handlers.h"


constexpr double AsNumber(const Value& val) {
    if (auto* num = std::get_if<double>(&val.data)) {
        return *num;
    }
    if (auto* boolean = std::get_if<bool>(&val.data)) {
        return *boolean ? 1.0 : 0.0;
    }
    throw EvaluatorErrors(EvaluatorErrors::kInvalidOperand);
}


constexpr bool IsTrue(const Value& val) {
    if (auto* boolean = std::get_if<bool>(&val.data)) {
        return *boolean;
    }
    if (auto* num = std::get_if<double>(&val.data)) {
        return *num != 0.0;
    }
    if (auto* str = std::get_if<std::string>(&val.data)) {
        return !str->empty();
    }
    if (std::holds_alternative<NilType>(val.data)) {
        return false;
    }
    return true;
}


Value Add(const Value& left, const Value& right) {
    if (auto* str1 = std::get_if<std::string>(&left.data)) {
        if (auto* str2 = std::get_if<std::string>(&right.data)) {
            return Value(*str1 + *str2);
        }
    }
    return Value(AsNumber(left) + AsNumber(right));
}


Value Substract(const Value& left, const Value& right) {
    if (auto* str1 = std::get_if<std::string>(&left.data)) {
        if (auto* str2 = std::get_if<std::string>(&right.data)) {
            std::string result = *str1;
            if (result.ends_with(*str2)) {
                result.erase(result.size() - str2->size());
            }
            return Value(result);
        }
    }

    return Value(AsNumber(left) - AsNumber(right));
}


Value Multiply(const Value& left, const Value& right) {
    if (auto* str = std::get_if<std::string>(&left.data)) {
        int times = static_cast<int>(std::floor(AsNumber(right)));
        std::string result;
        result.reserve(str->size() * times);
        for (int i = 0; i < times; ++i) {
            result += *str;
        }
        return Value(result);
    }

    if (auto* str = std::get_if<std::string>(&right.data)) {
        int times = static_cast<int>(std::floor(AsNumber(left)));
        std::string result;
        result.reserve(str->size() * times);
        for (int i = 0; i < times; ++i) {
            result += *str;
        }
        return Value(result);
    }

    return Value(AsNumber(left) * AsNumber(right));
}


Value Divide(const Value& left, const Value& right) {
    return Value(AsNumber(left) / AsNumber(right));
}


Value Mod(const Value& left, const Value& right) {
    return Value(std::fmod(AsNumber(left), AsNumber(right)));
}


Value PowerOf(const Value& left, const Value& right) {
    return Value(std::pow(AsNumber(left), AsNumber(right)));
}


Value Less(const Value& left, const Value& right) {
    return Compare(left, right, std::less{});
}


Value LessEqual(const Value& left, const Value& right) {
    return Compare(left, right, std::less_equal{});
}


Value Greater(const Value& left, const Value& right) {
    return Compare(left, right, std::greater{});
}


Value GreaterEqual(const Value& left, const Value& right) {
    return Compare(left, right, std::greater_equal{});
}


Value Negate(const Value& operand) {
    return Value(-AsNumber(operand));
}


Value LogicalNot(const Value& operand) {
    return Value(!IsTrue(operand));
}


void Initialize() {
    auto& registry = OperationRegistry::Get();

    registry.AddBinary(TokenType::plus_, Add);
    registry.AddBinary(TokenType::minus_, Substract);
    registry.AddBinary(TokenType::star_, Multiply);
    registry.AddBinary(TokenType::slash_, Divide);
    registry.AddBinary(TokenType::percent_, Mod);
    registry.AddBinary(TokenType::degree_, PowerOf);
    registry.AddBinary(TokenType::less_, Less);
    registry.AddBinary(TokenType::less_eq_, LessEqual);
    registry.AddBinary(TokenType::greater_, Greater);
    registry.AddBinary(TokenType::greater_eq_, GreaterEqual);
    registry.AddUnary(TokenType::minus_, Negate);
    registry.AddUnary(TokenType::not_, LogicalNot);
}
