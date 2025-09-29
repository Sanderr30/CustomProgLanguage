#include <gtest/gtest.h>
#include <sstream>
#include <runtime/interpreter/interpreter.h>

class CompleteBuiltinFunctionsTest : public ::testing::Test {
protected:
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
};


TEST_F(CompleteBuiltinFunctionsTest, MathFunctionsComplete) {
    std::string code = R"(
        print(abs(-5))
        print(abs(5))
        print(abs(-3.14))
        print(abs(0))

        print(ceil(3.14))
        print(ceil(3.0))
        print(ceil(-2.1))

        print(floor(3.14))
        print(floor(3.9))
        print(floor(-2.1))

        print(round(3.14))
        print(round(3.6))
        print(round(-2.4))
        print(round(-2.6))

        print(sqrt(16))
        print(sqrt(25))
        print(sqrt(2))
    )";

    EXPECT_TRUE(interpret(code));
}


TEST_F(CompleteBuiltinFunctionsTest, StringFunctionsComplete) {
    std::string code = R"(
        text = "Hello World ITMOScript Programming"

        print(len(text))
        print(len(""))
        print(len("a"))

        print(lower(text))
        print(lower("UPPERCASE"))
        print(lower("MixedCase123"))

        print(upper(text))
        print(upper("lowercase"))
        print(upper("MixedCase123"))

        words = split(text, " ")
        print(len(words))
        print(words[0])
        print(words[1])

        csv = "apple,banana,orange"
        fruits = split(csv, ",")
        print(len(fruits))
        print(fruits[0])
        print(fruits[2])

        joined_spaces = join(words, " ")
        print(joined_spaces)

        joined_dashes = join(fruits, "-")
        print(joined_dashes)

        replaced1 = replace(text, "World", "Universe")
        print(replaced1)

        replaced2 = replace(text, "o", "0")
        print(replaced2)

        replaced3 = replace(text, "xyz", "123")
        print(replaced3)
    )";

    EXPECT_TRUE(interpret(code));
}


TEST_F(CompleteBuiltinFunctionsTest, IOFunctions) {
    std::string code = R"(
        print("Hello")
        print(" ")
        print("World")
        print(42)
        print(true)
        print(nil)

        print([1, 2, 3])
        print(function() return 1 end function)
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(CompleteBuiltinFunctionsTest, PrintlnFunction) {
    std::string code = R"(
        println("Line 1")
        println("Line 2")
        println(42)
        println(true)
        println([1, 2, 3])
    )";

    EXPECT_TRUE(interpret(code));
}


TEST_F(CompleteBuiltinFunctionsTest, StacktraceFunction) {
    std::string code = R"(
        trace1 = stacktrace()
        print(type(trace1))

        func1 = function()
            return stacktrace()
        end function

        trace2 = func1()
        print(type(trace2))

        func3 = function()
            func4 = function()
                func5 = function()
                    return stacktrace()
                end function
                return func5()
            end function
            return func4()
        end function

        trace3 = func3()
        print(type(trace3))

        recursive_func = function(n)
            if n <= 0 then
                return stacktrace()
            else
                return recursive_func(n - 1)
            end if
        end function

        trace4 = recursive_func(3)
        print(type(trace4))
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(CompleteBuiltinFunctionsTest, FunctionEdgeCases) {
    std::string code = R"(
        print(abs(0))
        print(ceil(5.0))
        print(floor(5.0))
        print(round(2.5))
        print(sqrt(0))
        print(sqrt(1))

        print(len(""))
        print(upper(""))
        print(lower(""))

        empty_split = split("", ",")
        print(len(empty_split))

        empty_join = join([], "-")
        print(len(empty_join))

        no_replace = replace("hello", "x", "y")
        print(no_replace)

        empty_range = range(0)
        print(len(empty_range))

        single_range = range(1)
        print(len(single_range))
        print(single_range[0])

        negative_range = range(5, 2, -1)
        print(len(negative_range))

        empty_arr = []
        print(len(empty_arr))

        push(empty_arr, 1)
        print(len(empty_arr))

        sort([])

        single = [42]
        sort(single)
        print(single[0])
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(CompleteBuiltinFunctionsTest, FunctionErrorCases) {
    EXPECT_FALSE(interpret("print(sqrt(-1))"));
    EXPECT_FALSE(interpret("print(rnd(0))"));
    EXPECT_FALSE(interpret("print(rnd(-5))"));
    EXPECT_FALSE(interpret("print(abs())"));
    EXPECT_FALSE(interpret("print(abs(1, 2))"));
    EXPECT_FALSE(interpret("print(len())"));
    EXPECT_FALSE(interpret("print(len(1, 2))"));
    EXPECT_FALSE(interpret("print(len(42))"));
    EXPECT_FALSE(interpret("print(len(true))"));
    EXPECT_FALSE(interpret("print(split(123, \",\"))"));
    EXPECT_FALSE(interpret("print(join(\"not array\", \",\"))"));
    EXPECT_FALSE(interpret("push(\"not array\", 1)"));
    EXPECT_FALSE(interpret("pop(42)"));
    EXPECT_FALSE(interpret("empty = []; pop(empty)"));
    EXPECT_FALSE(interpret("arr = [1, 2]; insert(arr, 10, 3)"));
    EXPECT_FALSE(interpret("arr = [1, 2]; remove(arr, 10)"));
    EXPECT_FALSE(interpret("range(1, 10, 0)"));
    EXPECT_FALSE(interpret("range()"));
    EXPECT_FALSE(interpret("range(1, 2, 3, 4)"));
}
