#pragma once

#include <istream>

#include <token/token.h>
#include <rules/rules.h>


class Lexer {
public:
    Lexer(std::istream&);

    const Token& GetCurrentToken() const;

    Token ScanNextToken();

private:
    void Update();

    void SkipUnnecessary();

    Token ScanNextTokenImpl();

    Token ScanIdentifier();

    Token ScanNumber();

    Token ScanString();

private:
    std::istream& input_code_;
    std::size_t line_ = 1;
    std::size_t column_ = 0;
    int current_ {input_code_.get()};
    Token next_token_;
};
