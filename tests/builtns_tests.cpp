#include <gtest/gtest.h>
#include <sstream>
#include <runtime/interpreter/interpreter.h>

class TypesAndBuiltinsTest : public ::testing::Test {
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

TEST_F(TypesAndBuiltinsTest, TypeFunction) {
    std::string code = R"(
        print(type(42))
        print(type(3.14))
        print(type("hello"))
        print(type(true))
        print(type(false))
        print(type(nil))
        print(type([1, 2, 3]))
        print(type(function() return 1 end function))
    )";

    std::string expected = "numbernumberstringbooleanbooleannilarrayfunction";
    EXPECT_EQ(interpret_with_output(code), expected);
}

TEST_F(TypesAndBuiltinsTest, StringFunctions) {
    std::string code = R"(
        text = "Hello World ITMOScript"

        print(len(text))
        print(upper(text))
        print(lower(text))

        words = split(text, " ")
        print(len(words))

        joined = join(words, "-")
        print(joined)

        replaced = replace(text, "World", "Universe")
        print(replaced)
    )";

    std::string expected = "22\"HELLO WORLD ITMOSCRIPT\"\"hello world itmoscript\"3Hello-World-ITMOScript\"Hello Universe ITMOScript\"";
    EXPECT_EQ(interpret_with_output(code), expected);
}

TEST_F(TypesAndBuiltinsTest, MathFunctions) {
    std::string code = R"(
        print(abs(-10))
        print(abs(10))
        print(ceil(3.14))
        print(floor(3.14))
        print(round(3.14))
        print(round(3.64))
        print(sqrt(16))
        print(sqrt(2))
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(TypesAndBuiltinsTest, ArrayFunctions) {
    std::string code = R"(
        arr = [3, 1, 4, 1, 5, 9, 2, 6]

        print(len(arr))

        push(arr, 7)
        print(len(arr))

        last = pop(arr)
        print(last)
        print(len(arr))

        insert(arr, 2, 99)
        print(arr[2])

        remove(arr, 2)
        print(len(arr))

        sort(arr)
        print(arr[0])
        print(arr[7])
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(TypesAndBuiltinsTest, RangeFunction) {
    std::string code = R"(
        r1 = range(5)
        print(len(r1))
        print(r1[0])
        print(r1[4])

        r2 = range(2, 8)
        print(len(r2))
        print(r2[0])
        print(r2[5])

        r3 = range(0, 10, 2)
        print(len(r3))
        print(r3[0])
        print(r3[4])
    )";

    EXPECT_EQ(interpret_with_output(code), "504627508");
}

TEST_F(TypesAndBuiltinsTest, TypeConversion) {
    std::string code = R"(
        num = 42
        str_num = to_string(num)
        print(type(str_num))
        print(str_num)

        str = "123.45"
        parsed = parse_num(str)
        print(type(parsed))
        print(parsed)

        invalid = parse_num("not a number")
        print(invalid == nil)
    )";

    EXPECT_EQ(interpret_with_output(code), "string42number123.45true");
}

TEST_F(TypesAndBuiltinsTest, SliceOperations) {
    std::string code = R"(
        text = "ITMOScript"
        arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

        print(text[0])
        print(text[-1])
        print(text[:4])
        print(text[4:])
        print(text[2:6])
        print(text[:])

        print(len(arr[:5]))
        print(len(arr[5:]))
        print(len(arr[2:8]))
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(TypesAndBuiltinsTest, ComparisonOperations) {
    std::string code = R"(
        print(5 > 3)
        print(3 >= 3)
        print(2 < 5)
        print(4 <= 4)
        print(5 == 5)
        print(5 != 4)

        print("apple" < "banana")
        print("hello" == "hello")
        print("world" != "word")

        print(nil == nil)
        print(nil != 5)
        print("test" != nil)
    )";

    EXPECT_EQ(interpret_with_output(code), "truetruetruetruetruetruetruetruetruetruetruetrue");
}

TEST_F(TypesAndBuiltinsTest, OperatorPrecedence) {
    std::string code = R"(
        result1 = 2 + 3 * 4
        print(result1)

        result2 = (2 + 3) * 4
        print(result2)

        result3 = 2 ^ 3 * 2
        print(result3)

        result4 = true or false and false
        print(result4)

        result5 = (true or false) and false
        print(result5)
    )";

    EXPECT_EQ(interpret_with_output(code), "142016truefalse");
}

class EdgeCasesTest : public ::testing::Test {
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


TEST_F(EdgeCasesTest, NumberEdgeCases) {
    std::string code = R"(
        big_num = 1e15
        print(type(big_num))
        small_num = 1e-15
        print(type(small_num))

        zero1 = 0
        zero2 = 0.0
        zero3 = -0.0

        print(zero1 == zero2)
        print(zero2 == zero3)
    )";

    EXPECT_TRUE(interpret(code));
}


TEST_F(EdgeCasesTest, StringEdgeCases) {
    std::string code = R"(
        empty = ""
        print(len(empty))
        print(empty == "")

        spaces = "   "
        print(len(spaces))

        special = "\t\n\r"
        print(len(special))

        long_str = "a" * 1000
        print(len(long_str))

        result1 = empty + "hello"
        result2 = "hello" + empty
        print(result1)
        print(result2)

        result3 = "hello" * 0
        print(len(result3))
    )";

    EXPECT_TRUE(interpret(code));
}

TEST_F(EdgeCasesTest, FunctionEdgeCases) {
    std::string code = R"(
        no_params = function()
            return 42
        end function

        print(no_params())

        many_params = function(a, b, c, d, e)
            return a + b + c + d + e
        end function

        print(many_params(1, 2, 3, 4, 5))

        countdown = function(n)
            if n <= 0 then
                return 0
            else
                print(n)
                return countdown(n - 1)
            end if
        end function

        countdown(5)
    )";

    EXPECT_TRUE(interpret(code));
}


TEST_F(EdgeCasesTest, MemoryManagement) {
    std::string code = R"(
        for i in range(100)
            temp_array = range(10)
            temp_string = "test" * 10
            temp_func = function() return i end function
        end for

        print("memory_test_complete")
        arr1 = [1, 2, 3]
        arr2 = [4, 5, 6]

        print("reference_test_complete")
    )";

    EXPECT_EQ(interpret_with_output(code), "memory_test_completereference_test_complete");
}
