#include <gtest/gtest.h>
#include <sstream>

#include <lexer.h>
#include <token/token.h>
#include <errors/lex_errors.h>


TEST(Lexer, SkipComments) {
    std::istringstream in("//hello\nbar");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().lexeme, "bar");
}

TEST(Lexer, EndOfFile) {
    std::istringstream in("");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::end_of_file_);
    EXPECT_EQ(t.lexeme, "");
    EXPECT_EQ(t.line, 1);
    EXPECT_EQ(t.column, 0);
}

TEST(Lexer, Plus) {
    std::istringstream in("+");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::plus_);
}

TEST(Lexer, Minus) {
    std::istringstream in("-");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::minus_);
}

TEST(Lexer, Star) {
    std::istringstream in("*=");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::star_eq_);
}

TEST(Lexer, DobuleEq) {
    std::istringstream in("==");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::double_eq_);
}

TEST(Lexer, NotEq) {
    std::istringstream in("!=");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::not_eq_);
}

TEST(Lexer, LessEq) {
    std::istringstream in("<=");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::less_eq_);
}

TEST(Lexer, GreaterEq) {
    std::istringstream in(">=");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::greater_eq_);
}

TEST(Lexer, Identifier) {
    std::istringstream in("blablabla");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::identifier_);
    EXPECT_EQ(t.lexeme, "blablabla");
}

TEST(Lexer, IntegerNumber) {
    std::istringstream in("123");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "123");
}

TEST(Lexer, FloatNumber) {
    std::istringstream in("3.14");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "3.14");
}

TEST(Lexer, ExpNumber) {
    std::istringstream in("1e-5");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "1e-5");
}

TEST(Lexer, SimpleNumber) {
    std::istringstream in("\"hi\"");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::string_);
    EXPECT_EQ(t.lexeme, "hi");
}

TEST(Lexer, BooleanTrue) {
    std::istringstream in("true");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::boolean_);
    EXPECT_EQ(t.lexeme, "true");
}

TEST(Lexer, BooleanFalse) {
    std::istringstream in("false");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::boolean_);
    EXPECT_EQ(t.lexeme, "false");
}

TEST(Lexer, Nil) {
    std::istringstream in("nil");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::nil_);
    EXPECT_EQ(t.lexeme, "nil");
}

TEST(Lexer, KeyWordIf) {
    std::istringstream in("if");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::if_);
}

TEST(Lexer, KeyWordThen) {
    std::istringstream in("then");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::then_);
}

TEST(Lexer, KeyWordElse) {
    std::istringstream in("else");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::else_);
}

TEST(Lexer, LParen) {
    std::istringstream in("(");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::l_paren_);
}

TEST(Lexer, RParen) {
    std::istringstream in(")");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::r_paren_);
}

TEST(Lexer, Comma) {
    std::istringstream in(",");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::comma_);
}

TEST(Lexer, SkipWhiteSpace) {
    std::istringstream in("   \n\tfoo");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().lexeme, "foo");
}

TEST(Lexer, SkipNewLine) {
    std::istringstream in("foo\nbar");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().lexeme, "foo");
    EXPECT_EQ(lex.ScanNextToken().lexeme, "bar");
}

TEST(Lexer, SkipTabs) {
    std::istringstream in("foo\tbar");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().lexeme, "foo");
    EXPECT_EQ(lex.ScanNextToken().lexeme, "bar");
}

TEST(Lexer, KeyWordEnd) {
    std::istringstream in("end");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::end_);
}

TEST(Lexer, KeyWordWhile) {
    std::istringstream in("while");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::while_);
}

TEST(Lexer, KeyWordFor) {
    std::istringstream in("for");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::for_);
}

TEST(Lexer, KeyWordIn) {
    std::istringstream in("in");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::in_);
}

TEST(Lexer, KeyWordFunction) {
    std::istringstream in("function");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::function_);
}

TEST(Lexer, KeyWordReturn) {
    std::istringstream in("return");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::return_);
}

TEST(Lexer, KeyWordAnd) {
    std::istringstream in("and");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::and_);
}

TEST(Lexer, KeyWordOr) {
    std::istringstream in("or");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::or_);
}

TEST(Lexer, KeyWordNot) {
    std::istringstream in("not");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::not_);
}

TEST(Lexer, KeyWordBreak) {
    std::istringstream in("break");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::break_);
}

TEST(Lexer, KeyWordContinue) {
    std::istringstream in("continue");
    Lexer lex(in);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::continue_);
}

TEST(Lexer, IdentifierWithUnderscore) {
    std::istringstream in("_variable");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::identifier_);
    EXPECT_EQ(t.lexeme, "_variable");
}

TEST(Lexer, IdentifierWithNumbers) {
    std::istringstream in("var123");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::identifier_);
    EXPECT_EQ(t.lexeme, "var123");
}

TEST(Lexer, IdentifierMixedCase) {
    std::istringstream in("MyVariable_123");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::identifier_);
    EXPECT_EQ(t.lexeme, "MyVariable_123");
}

TEST(Lexer, FloatWithZero) {
    std::istringstream in("0.0");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "0.0");
}

TEST(Lexer, ExpNumberPositive) {
    std::istringstream in("1e+5");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "1e+5");
}

TEST(Lexer, ExpNumberCapitalE) {
    std::istringstream in("2.5E-3");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "2.5E-3");
}

TEST(Lexer, LargeNumber) {
    std::istringstream in("123456789");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "123456789");
}

TEST(Lexer, SmallFloat) {
    std::istringstream in("0.001");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::number_);
    EXPECT_EQ(t.lexeme, "0.001");
}

TEST(Lexer, StringWithNewline) {
    std::istringstream in("\"hello\\nworld\"");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::string_);
    EXPECT_EQ(t.lexeme, "hello\nworld");
}

TEST(Lexer, StringWithTab) {
    std::istringstream in("\"hello\\tworld\"");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::string_);
    EXPECT_EQ(t.lexeme, "hello\tworld");
}

TEST(Lexer, StringWithQuote) {
    std::istringstream in("\"hello\\\"world\"");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::string_);
    EXPECT_EQ(t.lexeme, "hello\"world");
}

TEST(Lexer, StringWithBackslash) {
    std::istringstream in("\"hello\\\\world\"");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::string_);
    EXPECT_EQ(t.lexeme, "hello\\world");
}

TEST(Lexer, EmptyString) {
    std::istringstream in("\"\"");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::string_);
    EXPECT_EQ(t.lexeme, "");
}

TEST(Lexer, LineColumnTracking) {
    std::istringstream in("hello\nworld");
    Lexer lex(in);
    Token t1 = lex.ScanNextToken();
    EXPECT_EQ(t1.line, 1);
    EXPECT_EQ(t1.column, 5);

    Token t2 = lex.ScanNextToken();
    EXPECT_EQ(t2.line, 2);
    EXPECT_EQ(t2.column, 5);
}

TEST(Lexer, MultilineTracking) {
    std::istringstream in("a\nb\nc");
    Lexer lex(in);

    Token t1 = lex.ScanNextToken();
    EXPECT_EQ(t1.line, 1);
    EXPECT_EQ(t1.lexeme, "a");

    Token t2 = lex.ScanNextToken();
    EXPECT_EQ(t2.line, 2);
    EXPECT_EQ(t2.lexeme, "b");

    Token t3 = lex.ScanNextToken();
    EXPECT_EQ(t3.line, 3);
    EXPECT_EQ(t3.lexeme, "c");
}


TEST(Lexer, TokenSequence) {
    std::istringstream in("if x == 5 then");
    Lexer lex(in);

    EXPECT_EQ(lex.ScanNextToken().type, TokenType::if_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::double_eq_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::number_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::then_);
}

TEST(Lexer, ArithmeticExpression) {
    std::istringstream in("a + b * 3.14");
    Lexer lex(in);

    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::plus_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::star_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::number_);
}

TEST(Lexer, OnlyWhitespace) {
    std::istringstream in("   \n\t  \n  ");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::end_of_file_);
}

TEST(Lexer, SingleCharacter) {
    std::istringstream in("x");
    Lexer lex(in);
    Token t = lex.ScanNextToken();
    EXPECT_EQ(t.type, TokenType::identifier_);
    EXPECT_EQ(t.lexeme, "x");
}

TEST(Lexer, FunctionDefinition) {
    std::istringstream in("function add(a, b) return a + b end");
    Lexer lex(in);

    EXPECT_EQ(lex.ScanNextToken().type, TokenType::function_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::l_paren_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::comma_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::r_paren_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::return_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::plus_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::end_);
}

TEST(Lexer, LogicalOperators) {
    std::istringstream in("if a and b or not c then");
    Lexer lex(in);

    EXPECT_EQ(lex.ScanNextToken().type, TokenType::if_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::and_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::or_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::not_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::identifier_);
    EXPECT_EQ(lex.ScanNextToken().type, TokenType::then_);
}

TEST(Lexer, GetCurrentToken) {
    std::istringstream in("hello world");
    Lexer lex(in);
    const Token& current = lex.GetCurrentToken();
    EXPECT_EQ(current.type, TokenType::identifier_);
    EXPECT_EQ(current.lexeme, "hello");
    Token next = lex.ScanNextToken();
    EXPECT_EQ(next.type, TokenType::identifier_);
    EXPECT_EQ(next.lexeme, "hello");
    const Token& current2 = lex.GetCurrentToken();
    EXPECT_EQ(current2.type, TokenType::identifier_);
    EXPECT_EQ(current2.lexeme, "world");
}
