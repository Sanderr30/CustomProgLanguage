#include <gtest/gtest.h>
#include <sstream>

#include <runtime/interpreter/interpreter.h>
#include <runtime/value/value.h>
#include <runtime/enviroment/enviroment.h>
#include <runtime/function/function.h>


bool interpret(const std::string& code) {
    std::istringstream input(code);
    std::ostringstream output;
    return Interpreter::Interpret(input, output);
}

std::string interpret_with_output(const std::string& code) {
    std::istringstream input(code);
    std::ostringstream output;
    if (Interpreter::Interpret(input, output)) {
        return output.str();
    }
    return "";
}


class ValueTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(ValueTest, NumberCreation) {
    Value v(42.0);
    EXPECT_TRUE(v.IsNumber());
    EXPECT_FALSE(v.IsString());
    EXPECT_EQ(v.AsNumber(), 42.0);
}

TEST_F(ValueTest, BoolCreation) {
    Value v(true);
    EXPECT_TRUE(v.IsBool());
    EXPECT_FALSE(v.IsNumber());
    EXPECT_EQ(v.AsBool(), true);
}

TEST_F(ValueTest, NilCreation) {
    Value v(NilType{});
    EXPECT_TRUE(v.IsNil());
    EXPECT_FALSE(v.IsNumber());
}

TEST_F(ValueTest, ArrayCreation) {
    Value::Array arr;
    arr.push_back(std::make_shared<Value>(1.0));
    arr.push_back(std::make_shared<Value>("test"));

    Value v(arr);
    EXPECT_TRUE(v.IsList());
    EXPECT_EQ(v.AsList().size(), 2);
}


class EnvironmentTest : public ::testing::Test {
protected:
    void SetUp() override {
        env = std::make_unique<Enviroment>();
    }

    std::unique_ptr<Enviroment> env;
};

TEST_F(EnvironmentTest, DefineVariable) {
    EXPECT_TRUE(env->Define("x", Value(42.0)));
    EXPECT_FALSE(env->Define("x", Value(24.0)));
}

TEST_F(EnvironmentTest, GetVariable) {
    env->Define("x", Value(42.0));
    Value result = env->Get("x");
    EXPECT_TRUE(result.IsNumber());
    EXPECT_EQ(result.AsNumber(), 42.0);
}

TEST_F(EnvironmentTest, AssignVariable) {
    env->Define("x", Value(42.0));
    EXPECT_TRUE(env->Assign("x", Value(24.0)));
    EXPECT_EQ(env->Get("x").AsNumber(), 24.0);
}

TEST_F(EnvironmentTest, AssignUndefinedVariable) {
    EXPECT_FALSE(env->Assign("undefined", Value(42.0)));
}

TEST_F(EnvironmentTest, UndefinedVariableThrows) {
    EXPECT_THROW(env->Get("undefined"), EnviromentError);
}


class InterpreterTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(InterpreterTest, SimpleAssignment) {
    EXPECT_TRUE(interpret("x = 42"));
}

TEST_F(InterpreterTest, PrintNumber) {
    EXPECT_EQ(interpret_with_output("print(42)"), "42");
}

TEST_F(InterpreterTest, PrintBool) {
    EXPECT_EQ(interpret_with_output("print(true)"), "true");
    EXPECT_EQ(interpret_with_output("print(false)"), "false");
}

TEST_F(InterpreterTest, PrintNil) {
    EXPECT_EQ(interpret_with_output("print(nil)"), "nil");
}

TEST_F(InterpreterTest, ArithmeticOperations) {
    EXPECT_EQ(interpret_with_output("print(2 + 3)"), "5");
    EXPECT_EQ(interpret_with_output("print(5 - 2)"), "3");
    EXPECT_EQ(interpret_with_output("print(3 * 4)"), "12");
    EXPECT_EQ(interpret_with_output("print(8 / 2)"), "4");
}

TEST_F(InterpreterTest, VariableOperations) {
    std::string code = R"(
        x = 10
        y = 20
        z = x + y
        print(z)
    )";
    EXPECT_EQ(interpret_with_output(code), "30");
}


class BuiltinTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(BuiltinTest, LenFunction) {
    EXPECT_EQ(interpret_with_output("print(len(\"hello\"))"), "5");
    EXPECT_EQ(interpret_with_output("print(len([1, 2, 3]))"), "3");
}

TEST_F(BuiltinTest, MathFunctions) {
    EXPECT_EQ(interpret_with_output("print(abs(-5))"), "5");
    EXPECT_EQ(interpret_with_output("print(abs(5))"), "5");
    EXPECT_EQ(interpret_with_output("print(min(3, 5, 1, 8))"), "1");
    EXPECT_EQ(interpret_with_output("print(max(3, 5, 1, 8))"), "8");
}

TEST_F(BuiltinTest, ArrayFunctions) {
    EXPECT_EQ(interpret_with_output("print(len(range(5)))"), "5");
    EXPECT_EQ(interpret_with_output("print(len(range(2, 8)))"), "6");
    EXPECT_EQ(interpret_with_output("print(len(range(0, 10, 2)))"), "5");
}


class FunctionTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(FunctionTest, SimpleFunctionCall) {
    std::string code = R"(
        f = function(x)
            return x * 2
        end function
        print(f(5))
    )";
    EXPECT_EQ(interpret_with_output(code), "10");
}

TEST_F(FunctionTest, FunctionWithMultipleParams) {
    std::string code = R"(
        add = function(a, b)
            return a + b
        end function
        print(add(3, 7))
    )";
    EXPECT_EQ(interpret_with_output(code), "10");
}

TEST_F(FunctionTest, FunctionWithoutReturn) {
    std::string code = R"(
        greet = function(name)
            print("Hello, " + name)
        end function
        greet("World")
    )";
    EXPECT_EQ(interpret_with_output(code), "\"Hello, World\"");
}


class ErrorTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(ErrorTest, UndefinedVariable) {
    EXPECT_FALSE(interpret("print(undefined_var)"));
}

TEST_F(ErrorTest, CallNonFunction) {
    EXPECT_FALSE(interpret("x = 42\nx()"));
}

TEST_F(ErrorTest, InvalidOperations) {
    EXPECT_FALSE(interpret("print(\"hello\" / \"world\")"));
    EXPECT_FALSE(interpret("print([1, 2] + 3)"));
}

TEST_F(ErrorTest, IndexOutOfBounds) {
    EXPECT_FALSE(interpret("s = \"hello\"\nprint(s[10])"));
    EXPECT_FALSE(interpret("arr = [1, 2, 3]\nprint(arr[5])"));
}
