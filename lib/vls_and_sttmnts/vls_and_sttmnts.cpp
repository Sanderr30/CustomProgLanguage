#include <vls_and_sttmnts.h>

NumberExpression::NumberExpression(double val)
    : value(val)
{}

StringExpression::StringExpression(const std::string& val)
    : value(val)
{}

BoolExpression::BoolExpression(bool val)
    : value(val)
{}

VariableExpression::VariableExpression(const std::string& nm)
    : name(nm)
{}
