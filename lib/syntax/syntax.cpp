#include <memory>
#include <algorithm>

#include <syntax.h>
#include <errors/synt_errors.h>


const std::unordered_map<TokenType, SyntaxAnalizer::primary_parser>
SyntaxAnalizer::primary_parsers_ = []() {
    std::unordered_map<TokenType, primary_parser> parsers;

    parsers[TokenType::number_] = [](const Token& token) -> Expression
    { return Expression { NumberExpression{std::stod(token.lexeme)} }; };

    parsers[TokenType::string_] = [](const Token& token) -> Expression
    { return Expression { StringExpression{token.lexeme} }; };

    parsers[TokenType::boolean_] = [](const Token& token) -> Expression
    { return Expression { BoolExpression{token.lexeme == "true"} }; };

    parsers[TokenType::nil_] = [](const Token&) -> Expression
    { return Expression { NilExpression{} }; };

    parsers[TokenType::identifier_] = [](const Token& tok) -> Expression
    { return Expression { VariableExpression{tok.lexeme} }; };

    return parsers;
} ();


SyntaxAnalizer::SyntaxAnalizer(std::istream& input)
    : lexer_(input)
    , current_tkn_(lexer_.ScanNextToken())
{
    InitializeStatementParsers();
}


std::vector<Statement> SyntaxAnalizer::Parse() {
    std::vector<Statement> program;
    while (current_tkn_.type != TokenType::end_of_file_) {
        program.push_back(ParseStatement());
    }
    return program;
}


void SyntaxAnalizer::Update() { current_tkn_ = lexer_.ScanNextToken(); }


bool SyntaxAnalizer::Match(TokenType t) {
    if (current_tkn_.type != t) { return false; }
    Update();
    return true;
}


void SyntaxAnalizer::Check(TokenType tkn) {
    if (current_tkn_.type != tkn) {
        throw SyntaxError(current_tkn_
        , SyntaxError::kExpectedToken);
    }
    Update();
    return;
}


void SyntaxAnalizer::InitializeStatementParsers() {
    statement_parsers_ =
    {
        {TokenType::if_, [this]() { Update(); return ParseIf(); } },
        {TokenType::while_, [this]() { Update(); return ParseWhile(); } },
        {TokenType::for_, [this]() { Update(); return ParseFor(); } },
        {TokenType::return_, [this]() { Update(); return ParseReturn(); } },
        {TokenType::break_, [this]() { Update(); return ParseBreak(); } },
        {TokenType::continue_, [this]() { Update(); return ParseContinue(); } }
    };
}


Statement SyntaxAnalizer::ParseStatement() {
    if (auto it = statement_parsers_.find(current_tkn_.type);
                it != statement_parsers_.end())
    {
        return it->second();
    }
    Expression expr = ParseExpression();
    return Statement{ ExpressionStatement {std::move(expr)} };
}


template<TokenType... Ops>
Expression SyntaxAnalizer::ParseBinaryLevel(auto parse_next) {
    Expression expression = parse_next();

    while ((... || (current_tkn_.type == Ops))) {
        TokenType op = current_tkn_.type;
        Update();
        if (current_tkn_.type == TokenType::end_of_file_) {
            throw SyntaxError(current_tkn_
            , SyntaxError::kUnexpectedEOF);
        }
        Expression right = parse_next();
        expression = Expression
        {
            BinaryExpression
            {
                std::make_unique<Expression>(std::move(expression))
                , op
                , std::make_unique<Expression>(std::move(right))
            }
        };
    }
    return expression;
}


Statement SyntaxAnalizer::ParseIf() {
    Expression condition = ParseExpression();
    Check(TokenType::then_);

    auto then_branch = ParseBlock({TokenType::else_, TokenType::end_});

    IfStatement head{ std::move(condition), std::move(then_branch), {} };
    IfStatement* current_if = &head;

    while (current_tkn_.type == TokenType::else_) {
        Update();
        if (current_tkn_.type == TokenType::if_) {
            Update();
            Expression else_condition = ParseExpression();
            Check(TokenType::then_);
            auto else_then = ParseBlock({TokenType::else_, TokenType::end_});
            current_if->else_case.push_back(
                Statement
                {
                    IfStatement
                    {
                        std::move(else_condition), std::move(else_then), {}
                    }
                }
            );

            auto& nested = std::get<IfStatement>(current_if->else_case.back().value);
            current_if = &nested;
        } else {
            current_if->else_case = ParseBlock({TokenType::end_});
            break;
        }
    }
    Check(TokenType::end_);
    Check(TokenType::if_);
    return Statement{std::move(head)};
}


Statement SyntaxAnalizer::ParseWhile() {
    Expression condition = ParseExpression();
    auto body = ParseBlock( {TokenType::end_} );
    Check(TokenType::end_);
    Check(TokenType::while_);
    return Statement{ WhileStatement{std::move(condition), std::move(body)} };
}


Statement SyntaxAnalizer::ParseFor() {
    if (current_tkn_.type != TokenType::identifier_) {
        throw SyntaxError(current_tkn_
        , SyntaxError::kExpectedIdentifierInFor);
    }

    std::string var = current_tkn_.lexeme;
    Update();
    Check(TokenType::in_);
    Expression iterable = ParseExpression();
    auto body = ParseBlock( {TokenType::end_} );
    Check(TokenType::end_);
    Check(TokenType::for_);

    return Statement{ ForStatement{std::move(var), std::move(iterable), std::move(body)} };
}


Statement SyntaxAnalizer::ParseReturn() {
    std::unique_ptr<Expression> value;
    if (current_tkn_.type != TokenType::end_
        && current_tkn_.type != TokenType::end_of_file_)
    {
        value = std::make_unique<Expression>(ParseExpression());
    }
    return Statement{ReturnStatement{std::move(value)}};
}


std::vector<Statement> SyntaxAnalizer::ParseBlock(std::initializer_list<TokenType> types) {
    std::vector<Statement> statements;
    while (current_tkn_.type != TokenType::end_of_file_ &&
        std::find(types.begin(), types.end(),
        current_tkn_.type) == types.end())
    {
        statements.push_back(ParseStatement());
    }

    if (current_tkn_.type == TokenType::end_of_file_) {
        throw SyntaxError(current_tkn_
        , SyntaxError::kUnexpectedEOF);
    }

    return statements;
}

Expression SyntaxAnalizer::ParseExpression() {
    return ParseAssignment();
}

Expression SyntaxAnalizer::ParseAssignment() {
    Expression expr = ParseOr();

    if (std::find(assignment_ops.begin(), assignment_ops.end(),
        current_tkn_.type) != assignment_ops.end())
    {
        TokenType op = current_tkn_.type;
        Update();
        auto rhs = std::make_unique<Expression>(ParseAssignment());

        if (!std::holds_alternative<VariableExpression>(expr.value)) {
            throw SyntaxError(current_tkn_
            , SyntaxError::kExpectedIdentifierInFor);
        }

        auto var = std::get<VariableExpression>(std::move(expr.value)).name;
        return Expression{ AssignExpression{std::move(var), op, std::move(rhs)} };
    }
    return expr;
}


Expression SyntaxAnalizer::ParseOr() {
    return ParseBinaryLevel<TokenType::or_>
    (
        [this]() { return ParseAnd(); }
    );
}


Expression SyntaxAnalizer::ParseAnd() {
    return ParseBinaryLevel<TokenType::and_>
    (
        [this]() { return ParseEquality(); }
    );
}


Expression SyntaxAnalizer::ParseEquality() {
    return ParseBinaryLevel<TokenType::double_eq_, TokenType::not_eq_>
    (
        [this]() { return ParseComparison(); }
    );
}


Expression SyntaxAnalizer::ParseComparison() {
    return ParseBinaryLevel<TokenType::less_, TokenType::less_eq_,
                           TokenType::greater_, TokenType::greater_eq_>
    (
        [this]() { return ParseTerm(); }
    );
}


Expression SyntaxAnalizer::ParseTerm() {
    return ParseBinaryLevel<TokenType::plus_, TokenType::minus_>
    (
        [this]() { return ParseFactor(); }
    );
}


Expression SyntaxAnalizer::ParseFactor() {
    return ParseBinaryLevel<TokenType::star_, TokenType::slash_,
                           TokenType::percent_, TokenType::degree_>
    (
        [this]() { return ParseUnary(); }
    );
}


Expression SyntaxAnalizer::ParseUnary() {
    constexpr std::array unary_ops =
    {
        TokenType::not_
        , TokenType::minus_
        , TokenType::plus_
    };

    if (std::find(unary_ops.begin(), unary_ops.end()
        , current_tkn_.type) != unary_ops.end())
    {
        TokenType op = current_tkn_.type;
        Update();
        Expression right = ParseUnary();
        return Expression{ UnaryExpression{op, std::make_unique<Expression>(std::move(right))} };
    }
    return ParseCallable();
}


Expression SyntaxAnalizer::ParseCallable() {
    Expression expr = ParsePrimary();

    while (true) {
        switch (current_tkn_.type) {
            case TokenType::l_paren_ : {
                Update();
                std::vector<std::unique_ptr<Expression>> args;
                if (current_tkn_.type != TokenType::r_paren_) {
                    if (current_tkn_.type != TokenType::r_paren_) {
                        args.push_back(std::make_unique<Expression>(ParseExpression()));
                        while (Match(TokenType::comma_)) {
                            args.push_back(std::make_unique<Expression>(ParseExpression()));
                        }
                    }
                }
                if (current_tkn_.type != TokenType::r_paren_) {
                    throw SyntaxError(current_tkn_
                    , SyntaxError::kExpectedRightParen);
                }
                Update();
                expr = Expression
                {
                    CallableExpression
                    {
                        std::make_unique<Expression>(std::move(expr)),
                        std::move(args)
                    }
                };
                break;
            }
            case TokenType::l_bracket_: {
                Update();
                if (current_tkn_.type == TokenType::colon_) {
                    Update();
                    std::unique_ptr<Expression> to;
                    if (current_tkn_.type != TokenType::r_bracket_) {
                        to = std::make_unique<Expression>(ParseExpression());
                    }
                    if (current_tkn_.type != TokenType::r_bracket_) {
                        throw SyntaxError(current_tkn_
                        , SyntaxError::kExpectedRightBracket);
                    }
                    Update();
                    expr = Expression {
                        SliceExpression {
                            std::make_unique<Expression>(std::move(expr)),
                            nullptr,
                            std::move(to)
                        }
                    };
                } else {
                    Expression from = ParseExpression();
                    if (Match(TokenType::colon_)) {
                        std::unique_ptr<Expression> to;
                        if (current_tkn_.type != TokenType::r_bracket_) {
                            to = std::make_unique<Expression>(ParseExpression());
                        }
                        if (current_tkn_.type != TokenType::r_bracket_) {
                            throw SyntaxError(current_tkn_
                            , SyntaxError::kExpectedRightBracket);
                        }
                        Update();
                        expr = Expression
                        {
                            SliceExpression {
                                std::make_unique<Expression>(std::move(expr)),
                                std::make_unique<Expression>(std::move(from)),
                                std::move(to)
                            }
                        };
                    } else {
                        if (current_tkn_.type != TokenType::r_bracket_) {
                            throw SyntaxError(current_tkn_
                            , SyntaxError::kExpectedRightBracket);
                        }
                        Update();
                        expr = Expression
                        {
                            IndexExpression {
                                std::make_unique<Expression>(std::move(expr)),
                                std::make_unique<Expression>(std::move(from))
                            }
                        };
                    }
                }
                break;
            }

            default:
                return expr;
        }
    }
}


Expression SyntaxAnalizer::ParsePrimary() {
    if (auto it = primary_parsers_.find(current_tkn_.type);
                            it != primary_parsers_.end()) {
        Token tok = current_tkn_;
        Update();
        return it->second(tok);
    }
    if (current_tkn_.type == TokenType::end_of_file_) {
        throw SyntaxError(current_tkn_
        , SyntaxError::kUnexpectedEOF);
    }
    return ParseComplexPrimary();
}


Expression SyntaxAnalizer::ParseComplexPrimary() {
    switch (current_tkn_.type) {
        case TokenType::l_bracket_: {
            Update();
            std::vector<std::unique_ptr<Expression>> elements;
            while (current_tkn_.type != TokenType::r_bracket_) {
                elements.push_back(std::make_unique<Expression>(ParseExpression()));
                if (!Match(TokenType::comma_)) { break; }
            }
            Check(TokenType::r_bracket_);
            return Expression{ ListExpression{std::move(elements)} };
        }
        case TokenType::function_: {
            Update();
            Check(TokenType::l_paren_);
            std::vector<std::string> params;
            if (current_tkn_.type != TokenType::r_paren_) {
                do {
                    if (current_tkn_.type != TokenType::identifier_) {
                        throw SyntaxError(current_tkn_
                        , SyntaxError::kExpectedIdentifierInFor);
                    }
                    params.push_back(current_tkn_.lexeme);
                    Update();
                } while (Match(TokenType::comma_));
            }
            if (current_tkn_.type != TokenType::r_paren_) {
                throw SyntaxError(current_tkn_
                , SyntaxError::kExpectedRightParen);
            }
            Update();
            auto body = ParseBlock({TokenType::end_});
            Check(TokenType::end_);
            Check(TokenType::function_);
            return Expression{ FunctionExpression{std::move(params), std::move(body)} };
        }
        case TokenType::l_paren_: {
            Update();
            Expression expr = ParseExpression();
            if (current_tkn_.type != TokenType::r_paren_) {
                throw SyntaxError(current_tkn_
                , SyntaxError::kExpectedRightParen);
            }
            Update();
            return expr;
        }
        default:
            throw SyntaxError(current_tkn_
            , SyntaxError::kUnexpectedTokenInPrimary);
    }
}


Statement SyntaxAnalizer::ParseBreak() {
    return Statement{ BreakStatement{} };
}


Statement SyntaxAnalizer::ParseContinue() {
    return Statement{ ContinueStatement{} };
}
