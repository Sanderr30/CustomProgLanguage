#include <gtest/gtest.h>
#include <sstream>

#include <syntax/syntax.h>


TEST(Parser, SimpleExpression) {
    std::istringstream in("1 + 2 * 3");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, SimpleExpressionWithParentheses) {
    std::istringstream in("(1 + 2) * 3");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, Assignment) {
    std::istringstream in("x = 5");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, IfThen) {
    std::istringstream in("if x then y end if");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, IfElse) {
    std::istringstream in("if x then y else z end if");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, While) {
    std::istringstream in("while x end while");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, For) {
    std::istringstream in("for i in [1,2] end for");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, Return) {
    std::istringstream in("return x");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, FunctionLiteral) {
    std::istringstream in("function(a, b) a + b end function");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, Call) {
    std::istringstream in("f(1, 2)");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, List) {
    std::istringstream in("[1,2,3]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, Unary) {
    std::istringstream in("-x");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, AndOr) {
    std::istringstream in("a and b or c");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(ParserError, MissingEndIf) {
    std::istringstream in("if x then y");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, BadFor) {
    std::istringstream in("for in x end for");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, BadCall) {
    std::istringstream in("f(,)");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, BadPrimary) {
    std::istringstream in(")");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(Parser, Complex) {
    std::istringstream in("x = function(a) if a > 0 then a * 2 else a / 2 end if end function");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NestedBlocks) {
    std::istringstream in("if x then while y for i in z end for end while else return end if");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(ParserError, UnterminatedFunction) {
    std::istringstream in("function() x");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(Parser, ManyOperators) {
    std::istringstream in("a + b - c * d / e % f ^ g");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(ParserError, BadAssignment) {
    std::istringstream in("1=2");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(Parser, NilLiteral) {
    std::istringstream in("nil");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, BooleanLiterals) {
    std::istringstream in("true false");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, StringLiteral) {
    std::istringstream in("\"hello world\"");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NumberLiterals) {
    std::istringstream in("42 3.14 1e-5");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, CompoundAssignments) {
    std::istringstream in("x += 5 y -= 3 z *= 2 a /= 4 b %= 3 c ^= 2");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, UnaryOperators) {
    std::istringstream in("-x +y not z");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NestedUnary) {
    std::istringstream in("not not x");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, UnaryWithParentheses) {
    std::istringstream in("-(x + y)");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ArithmeticPrecedence) {
    std::istringstream in("a + b * c");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, PowerOperator) {
    std::istringstream in("a ^ b ^ c");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ModuloOperator) {
    std::istringstream in("a % b");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ComparisonOperators) {
    std::istringstream in("a < b <= c > d >= e");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, EqualityOperators) {
    std::istringstream in("a == b != c");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, LogicalOperators) {
    std::istringstream in("a and b and c or d or e");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, EmptyFunction) {
    std::istringstream in("function() end function");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, SingleParameterFunction) {
    std::istringstream in("function(x) x end function");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, MultiParameterFunction) {
    std::istringstream in("function(a, b, c, d) a + b + c + d end function");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, FunctionWithMultipleStatements) {
    std::istringstream in("function(x) y = x * 2 return y end function");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, EmptyCall) {
    std::istringstream in("f()");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, SingleArgumentCall) {
    std::istringstream in("f(x)");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, MultiArgumentCall) {
    std::istringstream in("f(a, b, c, d)");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NestedFunctionCalls) {
    std::istringstream in("f(g(h(x)))");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ChainedCalls) {
    std::istringstream in("f()()()");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, EmptyList) {
    std::istringstream in("[]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, SingleElementList) {
    std::istringstream in("[42]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, MultiElementList) {
    std::istringstream in("[1, 2, 3, 4, 5]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NestedLists) {
    std::istringstream in("[[1, 2], [3, 4]]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ListWithExpressions) {
    std::istringstream in("[a + b, c * d, f(x)]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, SimpleIndex) {
    std::istringstream in("a[0]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ExpressionIndex) {
    std::istringstream in("a[b + c]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ChainedIndexing) {
    std::istringstream in("a[0][1][2]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, IndexingWithCalls) {
    std::istringstream in("f()[0]");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, WhileWithComplexCondition) {
    std::istringstream in("while a > 0 and not done end while");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NestedWhile) {
    std::istringstream in("while a while b end while end while");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NestedFor) {
    std::istringstream in("for i in list1 for j in list2 end for end for");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, EmptyReturn) {
    std::istringstream in("return");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ReturnExpression) {
    std::istringstream in("return a + b * c");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ReturnFunction) {
    std::istringstream in("return function(x) x * 2 end function");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ComplexArithmetic) {
    std::istringstream in("(a + b) * (c - d) / (e + f) ^ (g % h)");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, MixedOperatorPrecedence) {
    std::istringstream in("a or b and c == d < e + f * g ^ h");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ComplexFunctionExpression) {
    std::istringstream in("f(a + b, g(c), [d, e], h(i)[j])");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, MultipleStatements) {
    std::istringstream in("a = 1 b = 2 c = a + b");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, MixedStatements) {
    std::istringstream in("x = 5 if x > 0 then y = x * 2 end if return y");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(ParserError, UnexpectedToken) {
    std::istringstream in("+ + +");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, MissingRightParen) {
    std::istringstream in("(a + b");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, MissingRightBracket) {
    std::istringstream in("[1, 2, 3");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, MissingEndWhile) {
    std::istringstream in("while true x = 1");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, MissingEndFor) {
    std::istringstream in("for i in list x = i");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, MissingThen) {
    std::istringstream in("if x x = 1 end if");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, MissingIn) {
    std::istringstream in("for i list end for");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, InvalidAssignmentTarget) {
    std::istringstream in("(a + b) = 5");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, InvalidFunctionParameter) {
    std::istringstream in("function(1, 2) end function");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(ParserError, InvalidPrimary) {
    std::istringstream in("*");
    SyntaxAnalizer p(in);
    EXPECT_THROW(p.Parse(), std::runtime_error);
}

TEST(Parser, EmptyProgram) {
    std::istringstream in("");
    SyntaxAnalizer p(in);
    auto result = p.Parse();
    EXPECT_TRUE(result.empty());
}

TEST(Parser, SingleVariable) {
    std::istringstream in("x");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, DeepNesting) {
    std::istringstream in("((((((a))))))");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ComplexProgram) {
    std::istringstream in(R"(
        factorial = function(n)
            if n <= 1 then
                return 1
            else
                return n * factorial(n - 1)
            end if
        end function

        result = factorial(5)
        return result
    )");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, ListProcessing) {
    std::istringstream in(R"(
        data = [1, 2, 3, 4, 5]
        sum = 0
        for item in data
            sum += item
        end for
        return sum
    )");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, IfWithComplexCondition) {
    std::istringstream in("if a > 0 and b < 10 or c == nil then x end if");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}

TEST(Parser, NestedIf) {
    std::istringstream in("if a then if b then c end if end if");
    SyntaxAnalizer p(in);
    EXPECT_NO_THROW(p.Parse());
}
