#include <gtest/gtest.h>

#include <runtime/interpreter/interpreter.h>

TEST(TypesTestSuite, IntTest) {
    std::string code = R"(
        x = 1
        y = 2
        z = 3 * x + y
        print(z)
    )";

    std::string expected = "5";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(Interpreter::Interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}
