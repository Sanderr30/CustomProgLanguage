#include <sstream>

#include <value.h>
#include <errors/val_errors.h>


Value::Value()
    : data(NilType{})
{}

Value::Value(double val)
    : data(val)
{}

Value::Value(const std::string& val)
    : data(val)
{}

Value::Value(const char* str)
    : data(std::string(str))
{}

Value::Value(NilType val)
    : data(val)
{}

Value::Value(const Array& val)
    : data(val)
{}

Value::Value(FuncPtr val)
    : data(val)
{}


bool Value::IsNumber() const { return std::holds_alternative<double>(data); }

bool Value::IsString() const { return std::holds_alternative<std::string>(data); }

bool Value::IsBool() const { return std::holds_alternative<bool>(data); }

bool Value::IsNil() const { return std::holds_alternative<NilType>(data); }

bool Value::IsList() const { return std::holds_alternative<Array>(data); }

bool Value::IsFunction() const { return std::holds_alternative<FuncPtr>(data); }


double Value::AsNumber() const {
    if (auto* ptr = std::get_if<double>(&data)) {
        return *ptr;
    }
    throw ValueErrors(ValueErrors::kValueNotNumber);
}


const std::string& Value::AsString() const {
    if (auto* ptr = std::get_if<std::string>(&data)) {
        return *ptr;
    }
    throw ValueErrors(ValueErrors::kValueNotString);
}


bool Value::AsBool() const {
    if (auto* ptr = std::get_if<bool>(&data)) {
        return *ptr;
    }
    throw ValueErrors(ValueErrors::kValueNotBool);
}


const Value::Array& Value::AsList() const {
    if (auto* ptr = std::get_if<Array>(&data)) {
        return *ptr;
    }
    throw ValueErrors(ValueErrors::kValueNotList);
}


Value::Array& Value::AsList() {
    if (auto* ptr = std::get_if<Array>(&data)) {
        return *ptr;
    }
    throw ValueErrors(ValueErrors::kValueNotList);
}


Value::FuncPtr Value::AsFunction() const {
    if (auto* ptr = std::get_if<FuncPtr>(&data)) {
        return *ptr;
    }
    throw ValueErrors(ValueErrors::kValueNotFunction);
}


std::string Value::ToString() const {
    return std::visit([](const auto& val) -> std::string {
        using type = std::decay_t<decltype(val)>;

        if constexpr (std::is_same_v<type, double>) { return std::to_string(val); }
        else if constexpr (std::is_same_v<type, std::string>) { return val; }
        else if constexpr (std::is_same_v<type, bool>) { return val ? "true" : "false"; }
        else if constexpr (std::is_same_v<type, FuncPtr>) { return "<function>"; }
        else if constexpr (std::is_same_v<type, NilType>) { return "nil"; }
        else if constexpr (std::is_same_v<type, Array>) {
            std::stringstream ss;
            ss << "[ ";
            for (std::size_t i = 0; i < val.size(); ++i) {
                if (i > 0) {
                    ss << ", ";
                }
                ss << val[i]->ToString();
            }
            ss << "]";
            return ss.str();
        }
    }, data);
}


std::ostream& operator<<(std::ostream& os, const Value& val) {
    os << val.ToString();
    return os;
}
