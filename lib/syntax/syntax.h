#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <array>

#include <lexer.h>
#include <vls_and_sttmnts.h>
#include <errors/synt_errors.h>


class SyntaxAnalizer {
public:
    SyntaxAnalizer(std::istream&);
    std::vector<Statement> Parse();

private:
    void Update();
    bool Match(TokenType);
    void Check(TokenType);

private:
    using StatementParser = std::function<Statement()>;

private:
    std::unordered_map<TokenType, StatementParser> statement_parsers_;
    void InitializeStatementParsers();

    Statement ParseStatement();
    Statement ParseIf();
    Statement ParseWhile();
    Statement ParseFor();
    Statement ParseReturn();
    Statement ParseBreak();
    Statement ParseContinue();


    std::vector<Statement> ParseBlock(std::initializer_list<TokenType>);

private:
    template<TokenType... Ops>
    Expression ParseBinaryLevel(auto parse_next);

    using primary_parser = std::function<Expression(const Token&)>;

    static const std::unordered_map<TokenType, primary_parser> primary_parsers_;

    Expression ParseComplexPrimary();
    Expression ParseExpression();
    Expression ParseAssignment();
    Expression ParseOr();
    Expression ParseAnd();
    Expression ParseEquality();
    Expression ParseComparison();
    Expression ParseTerm();
    Expression ParseFactor();
    Expression ParseUnary();
    Expression ParseCallable();
    Expression ParsePrimary();

private:
    static constexpr std::array assignment_ops = {
        TokenType::assign_
        , TokenType::plus_eq_
        , TokenType::minus_eq_
        , TokenType::star_eq_
        , TokenType::slash_eq_
        , TokenType::percent_eq_
        , TokenType::degree_eq_
    };

private:
    Lexer lexer_;
    Token current_tkn_;
};
