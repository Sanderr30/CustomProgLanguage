#include <gtest/gtest.h>

#include <runtime/evaluator/operations/handlers.h>
#include <runtime/evaluator/operations/register.h>
#include <runtime/value/value.h>


class OperationHandlersTest : public ::testing::Test {
protected:
    void SetUp() override {
        Initialize();
    }
};

TEST_F(OperationHandlersTest, AsNumber) {
    EXPECT_EQ(AsNumber(Value(42.0)), 42.0);
    EXPECT_EQ(AsNumber(Value(true)), 1.0);
    EXPECT_EQ(AsNumber(Value(false)), 0.0);
    EXPECT_THROW(AsNumber(Value("hello")), EvaluatorErrors);
}

TEST_F(OperationHandlersTest, IsTrue) {
    EXPECT_TRUE(IsTrue(Value(true)));
    EXPECT_FALSE(IsTrue(Value(false)));
    EXPECT_TRUE(IsTrue(Value(42.0)));
    EXPECT_FALSE(IsTrue(Value(0.0)));
    EXPECT_TRUE(IsTrue(Value("hello")));
    EXPECT_FALSE(IsTrue(Value("")));
    EXPECT_FALSE(IsTrue(Value(NilType{})));
}

TEST_F(OperationHandlersTest, AddOperation) {

    Value result1 = Add(Value(3.0), Value(5.0));
    EXPECT_TRUE(result1.IsNumber());
    EXPECT_EQ(result1.AsNumber(), 8.0);

    Value result2 = Add(Value("hello"), Value(" world"));
    EXPECT_TRUE(result2.IsString());
    EXPECT_EQ(result2.AsString(), "hello world");

    Value result3 = Add(Value(3.0), Value(true));
    EXPECT_TRUE(result3.IsNumber());
    EXPECT_EQ(result3.AsNumber(), 4.0);
}

TEST_F(OperationHandlersTest, MultiplyOperation) {
    Value result1 = Multiply(Value(3.0), Value(4.0));
    EXPECT_TRUE(result1.IsNumber());
    EXPECT_EQ(result1.AsNumber(), 12.0);


    Value result2 = Multiply(Value("abc"), Value(3.0));
    EXPECT_TRUE(result2.IsString());
    EXPECT_EQ(result2.AsString(), "abcabcabc");


    Value result3 = Multiply(Value(2.0), Value("hi"));
    EXPECT_TRUE(result3.IsString());
    EXPECT_EQ(result3.AsString(), "hihi");
}

TEST_F(OperationHandlersTest, SubtractOperation) {
    Value result1 = Substract(Value(10.0), Value(3.0));
    EXPECT_TRUE(result1.IsNumber());
    EXPECT_EQ(result1.AsNumber(), 7.0);

    Value result2 = Substract(Value("hello world"), Value("world"));
    EXPECT_TRUE(result2.IsString());
    EXPECT_EQ(result2.AsString(), "hello ");

    Value result3 = Substract(Value("hello world"), Value("xyz"));
    EXPECT_TRUE(result3.IsString());
    EXPECT_EQ(result3.AsString(), "hello world");
}

TEST_F(OperationHandlersTest, CompareOperations) {

    EXPECT_TRUE(Less(Value(3.0), Value(5.0)).AsBool());
    EXPECT_FALSE(Less(Value(5.0), Value(3.0)).AsBool());

    EXPECT_TRUE(Less(Value("apple"), Value("banana")).AsBool());
    EXPECT_FALSE(Less(Value("banana"), Value("apple")).AsBool());

    EXPECT_TRUE(LessEqual(Value(3.0), Value(3.0)).AsBool());
    EXPECT_TRUE(GreaterEqual(Value(5.0), Value(5.0)).AsBool());
}

TEST_F(OperationHandlersTest, UnaryOperations) {
    Value result1 = Negate(Value(5.0));
    EXPECT_TRUE(result1.IsNumber());
    EXPECT_EQ(result1.AsNumber(), -5.0);

    Value result2 = LogicalNot(Value(true));
    EXPECT_TRUE(result2.IsBool());
    EXPECT_FALSE(result2.AsBool());

    Value result3 = LogicalNot(Value(0.0));
    EXPECT_TRUE(result3.IsBool());
    EXPECT_TRUE(result3.AsBool());
}

class OperationRegistryTest : public ::testing::Test {
protected:
    void SetUp() override {
        Initialize();
        registry = &OperationRegistry::Get();
    }

    OperationRegistry* registry;
};

TEST_F(OperationRegistryTest, BinaryOperationSupport) {
    EXPECT_TRUE(registry->SupportsBinary(TokenType::plus_));
    EXPECT_TRUE(registry->SupportsBinary(TokenType::minus_));
    EXPECT_TRUE(registry->SupportsBinary(TokenType::star_));
    EXPECT_TRUE(registry->SupportsBinary(TokenType::slash_));
    EXPECT_FALSE(registry->SupportsBinary(TokenType::identifier_));
}

TEST_F(OperationRegistryTest, UnaryOperationSupport) {
    EXPECT_TRUE(registry->SupportsUnary(TokenType::minus_));
    EXPECT_TRUE(registry->SupportsUnary(TokenType::not_));
    EXPECT_FALSE(registry->SupportsUnary(TokenType::plus_));
}

TEST_F(OperationRegistryTest, ExecuteBinaryOperations) {
    Value result = registry->ExecuteBinary(TokenType::plus_, Value(3.0), Value(5.0));
    EXPECT_TRUE(result.IsNumber());
    EXPECT_EQ(result.AsNumber(), 8.0);
}

TEST_F(OperationRegistryTest, ExecuteUnaryOperations) {
    Value result = registry->ExecuteUnary(TokenType::minus_, Value(5.0));
    EXPECT_TRUE(result.IsNumber());
    EXPECT_EQ(result.AsNumber(), -5.0);
}

TEST_F(OperationRegistryTest, UnsupportedOperations) {
    EXPECT_THROW(registry->ExecuteBinary(TokenType::identifier_, Value(1.0), Value(2.0)),
                 EvaluatorErrors);
    EXPECT_THROW(registry->ExecuteUnary(TokenType::plus_, Value(5.0)),
                 EvaluatorErrors);
}
