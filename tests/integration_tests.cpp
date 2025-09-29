#include <gtest/gtest.h>
#include <sstream>
#include <runtime/interpreter/interpreter.h>

class IntegrationTest : public ::testing::Test {
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


TEST_F(IntegrationTest, FibonacciFunction) {
    std::string code = R"(
        fib = function(n)
            if n == 0 then
                return 0
            end if

            a = 0
            b = 1

            for i in range(n - 1)
                c = a + b
                a = b
                b = c
            end for

            return b
        end function

        print(fib(0))
        print(fib(1))
        print(fib(5))
        print(fib(10))
    )";

    EXPECT_EQ(interpret_with_output(code), "01555");
}


TEST_F(IntegrationTest, MaximumFunction) {
    std::string code = R"(
        max = function(arr)
            if len(arr) == 0 then
                return nil
            end if

            m = arr[0]

            for i in arr
                if i > m then m = i end if
            end for

            return m
        end function

        print(max([10, -1, 0, 2, 2025, 239]))
        print(max([1]))
        print(max([-5, -1, -10]))
    )";

    EXPECT_EQ(interpret_with_output(code), "20251-1");
}

TEST_F(IntegrationTest, FizzBuzzFunction) {
    std::string code = R"(
        fizzBuzz = function(n)
            for i in range(1, n)
                s = ""
                if i % 3 == 0 then s = s + "Fizz" end if
                if i % 5 == 0 then s = s + "Buzz" end if
                if s == "" then
                    print(i)
                else
                    print(s)
                end if
            end for
        end function

        fizzBuzz(16)
    )";

    std::string expected = "12Fizz4BuzzFizz78FizzBuzz11Fizz1314FizzBuzz";
    EXPECT_EQ(interpret_with_output(code), expected);
}

TEST_F(IntegrationTest, StringOperations) {
    std::string code = R"(
        s1 = "Hello"
        s2 = " World"
        result = s1 + s2
        print(result)

        star = "*"
        line = star * 5
        print(line)

        text = "ITMOScript"
        print(len(text))
        print(upper(text))
        print(lower(text))
    )";

    EXPECT_EQ(interpret_with_output(code), "\"Hello World\"*****10ITMOSCRIPTitmoscript");
}

TEST_F(IntegrationTest, ArrayOperations) {
    std::string code = R"(
        arr = [1, 2, 3, 4, 5]
        print(len(arr))
        print(arr[0])
        print(arr[4])

        arr2 = [6, 7, 8]
        print(len(arr2))

        small = [1, 2]
        print(len(small))
    )";

    EXPECT_EQ(interpret_with_output(code), "51532");
}


TEST_F(IntegrationTest, ConditionalAndLoops) {
    std::string code = R"(
        for i in range(1, 11)
            if i % 2 == 0 then
                print("even")
            else
                print("odd")
            end if
        end for
    )";

    std::string expected = "oddevenoddevenoddevenoddevenoddeven";
    EXPECT_EQ(interpret_with_output(code), expected);
}

TEST_F(IntegrationTest, HigherOrderFunctions) {
    std::string code = R"(
        apply_twice = function(f, x)
            return f(f(x))
        end function

        double = function(n)
            return n * 2
        end function

        result = apply_twice(double, 5)
        print(result)

        square_twice = apply_twice(function(x) return x * x end function, 3)
        print(square_twice)
    )";

    EXPECT_EQ(interpret_with_output(code), "2081");
}

TEST_F(IntegrationTest, ScopeTest) {
    std::string code = R"(
        global_var = 100

        test_scope = function(local_param)
            local_var = 200
            global_var = 300
            return local_param + local_var
        end function

        result = test_scope(50)
        print(result)
        print(global_var)
    )";

    EXPECT_EQ(interpret_with_output(code), "250300");
}

TEST_F(IntegrationTest, RecursiveFunction) {
    std::string code = R"(
        factorial = function(n)
            if n <= 1 then
                return 1
            else
                return n * factorial(n - 1)
            end if
        end function

        print(factorial(1))
        print(factorial(5))
        print(factorial(6))
    )";

    EXPECT_EQ(interpret_with_output(code), "1120720");
}

TEST_F(IntegrationTest, MathFunctions) {
    std::string code = R"(
        numbers = [-5.7, 3.14, 0.5, 4.9]

        for num in numbers
            print(abs(num))
            print(floor(num))
            print(ceil(num))
            print(round(num))
        end for
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(IntegrationTest, ErrorHandling) {

    EXPECT_FALSE(interpret("arr = [1, 2, 3]\nprint(arr[10])"));

    EXPECT_FALSE(interpret("print(nonexistent_function())"));

    EXPECT_FALSE(interpret("print(len(42))"));
}

TEST_F(IntegrationTest, NilHandling) {
    std::string code = R"(
        x = nil
        print(x)
        print(x == nil)
        print(x != nil)

        get_nil = function()
            return nil
        end function

        result = get_nil()
        print(result == nil)
    )";

    EXPECT_EQ(interpret_with_output(code), "niltruefalsetrue");
}

TEST_F(IntegrationTest, ComplexExpressions) {
    std::string code = R"(
        result1 = (2 + 3) * (4 - 1) + 10 / 2
        print(result1)
        a = 5
        b = 10
        c = 15
        result2 = (a < b) and (b < c) and (a + b == c)
        print(result2)

        str_num = "The answer is " + to_string(42)
        print(str_num)
    )";
    EXPECT_EQ(interpret_with_output(code), "20true\"The answer is 42\"");
}

TEST_F(IntegrationTest, NestedStructures) {
    std::string code = R"(
        for i in range(1, 4)
            for j in range(1, 4)
                product = i * j
                if product > 5 then
                    print("big")
                else
                    print("small")
                end if
            end for
        end for
    )";

    std::string expected = "smallsmallsmallsmallsmallbigsmallbigbig";
    EXPECT_EQ(interpret_with_output(code), expected);
}

TEST_F(IntegrationTest, WordCountProgram) {
    std::string code = R"(
        count_words = function(text, word)
            words = split(text, " ")
            count = 0
            for w in words
                if w == word then
                    count = count + 1
                end if
            end for
            return count
        end function

        text = "hello world hello universe hello"
        count = count_words(text, "hello")
        print(count)
    )";

    EXPECT_EQ(interpret_with_output(code), "3");
}
