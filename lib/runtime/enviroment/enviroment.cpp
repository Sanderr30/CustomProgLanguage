#include <runtime/enviroment/enviroment.h>

Enviroment::Enviroment() = default;


Enviroment::Enviroment(Enviroment* parent)
    : parent_(parent)
{}


bool Enviroment::Define(const std::string& name, Value val) {
    if (values_.count(name)) {
        return false;
    }
    values_[name] = std::move(val);
    return true;
}


bool Enviroment::Assign(const std::string& name, Value val) {
    if (values_.count(name)) {
        values_[name] = std::move(val);
        return true;
    }
    if (parent_) {
        return parent_->Assign(name, std::move(val));
    }
    return false;
}


Value Enviroment::Get(const std::string& name) const {
    if (values_.count(name)) {
        return values_.at(name);
    }
    if (parent_) {
        return parent_->Get(name);
    }
    throw EnviromentError(EnviromentError::kUndefinedVariable + name);
}
