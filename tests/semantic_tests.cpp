#include <gtest/gtest.h>
#include <sstream>

#include <semantic.h>


bool analyze(const std::string& src) {
    std::istringstream in(src);
    auto ast = SyntaxAnalizer(in).Parse();
    std::ostringstream errs;
    return SemanticAnalizer(errs).Analyse(ast);
}

TEST(Semantic, VarDecl) {
    EXPECT_TRUE(analyze("x = 1"));
}

TEST(Semantic, VarUse) {
    EXPECT_TRUE(analyze("x = 1 \n print(x)"));
}

TEST(Semantic, Shadow) {
    EXPECT_TRUE(analyze("x=1 \n if x then x = 2 end if"));
}

TEST(Semantic, MultipleVarDecl) {
    EXPECT_TRUE(analyze("x = 1\n y = 2\n z = x + y"));
}

TEST(Semantic, StringOperations) {
    EXPECT_TRUE(analyze("s = \"hello\"\n t = \"world\"\n x = s + t"));
}

TEST(Semantic, StringLength) {
    EXPECT_TRUE(analyze("s = \"test\"\n x = len(s)"));
}

TEST(Semantic, StringMethods) {
    EXPECT_TRUE(analyze(
        "s = \"Hello World\"\n"
        "l = lower(s)\n"
        "u = upper(s)\n"
        "parts = split(s, \" \")"
    ));
}

TEST(Semantic, FuncParam) {
    EXPECT_TRUE(analyze("f = function(a) return a end function \n f(2)"));
}

TEST(Semantic, NestedFunc) {
    EXPECT_TRUE(analyze("g = function(a) f = function(b) return b end function end function"));
}

TEST(Semantic, MultipleFuncParams) {
    EXPECT_TRUE(analyze(
        "f = function(a, b, c)\n"
        "  return a + b + c\n"
        "end function\n"
        "x = f(1, 2, 3)"
    ));
}

TEST(Semantic, FuncReturnTypes) {
    EXPECT_TRUE(analyze(
        "f = function(x)\n"
        "  if x then\n"
        "    return \"string\"\n"
        "  else\n"
        "    return 42\n"
        "  end if\n"
        "end function"
    ));
}

TEST(Semantic, BuiltinCall) {
    EXPECT_TRUE(analyze("print(1)"));
}

TEST(Semantic, BuiltinMath) {
    EXPECT_TRUE(analyze(
        "x = abs(-5)\n"
        "y = ceil(3.14)\n"
        "z = floor(3.14)\n"
        "w = sqrt(16)"
    ));
}

TEST(Semantic, LenString) {
    EXPECT_TRUE(analyze("len(\"a\")"));
}

TEST(Semantic, LenArray) {
    EXPECT_TRUE(analyze("len([1,2])"));
}

TEST(Semantic, ArrayOperations) {
    EXPECT_TRUE(analyze(
        "arr = [1, 2, 3]\n"
        "push(arr, 4)\n"
        "x = pop(arr)\n"
        "insert(arr, 1, 5)\n"
        "remove(arr, 0)"
    ));
}


TEST(SemanticError, UndefinedVariable) {
    EXPECT_FALSE(analyze("print(x)"));
}

TEST(SemanticError, DuplicateParamNames) {
    EXPECT_FALSE(analyze(
        "f = function(x, x)\n"
        "  return x\n"
        "end function"
    ));
}

TEST(SemanticError, InvalidUnaryOp) {
    EXPECT_FALSE(analyze("x = -\"hello\""));
}


TEST(SemanticError, RightArgs) {
    EXPECT_TRUE(analyze("print(1,2)"));
}

TEST(SemanticError, BadLen) {
    EXPECT_TRUE(analyze("len(1)"));
}

TEST(SemanticError, WrongArgumentType) {
    EXPECT_FALSE(analyze("sqrt(\"hello\")"));
}

TEST(Semantic, IfSem) {
    EXPECT_TRUE(analyze("if true then x = 1 end if"));
}

TEST(Semantic, WhileSem) {
    EXPECT_TRUE(analyze("while true x = 1 end while"));
}

TEST(Semantic, ForSem) {
    EXPECT_TRUE(analyze(
        "arr = [1, 2, 3]\n"
        "for x in arr\n"
        "  print(x)\n"
        "end for"
    ));
}

TEST(Semantic, ScopeSem) {
    EXPECT_TRUE(analyze(
        "x = 1\n"
        "if true then\n"
        "  x = 2\n"
        "  y = 3\n"
        "end if\n"
        "x = 4"
    ));
}

TEST(Semantic, NestedScopeSem) {
    EXPECT_TRUE(analyze(
        "x = 1\n"
        "if true then\n"
        "  y = 2\n"
        "  if true then\n"
        "    z = 3\n"
        "    x = 4\n"
        "  end if\n"
        "end if"
    ));
}

TEST(Semantic, ReturnInFunc) {
    EXPECT_TRUE(analyze("f = function() return 1 end function"));
}

TEST(Semantic, NoReturn) {
    EXPECT_TRUE(analyze("f = function() x = 1 end function"));
}

TEST(Semantic, ConditionalReturn) {
    EXPECT_TRUE(analyze(
        "f = function(x)\n"
        "  if x then\n"
        "    return 1\n"
        "  end if\n"
        "  return 0\n"
        "end function"
    ));
}

TEST(Semantic, ComplexSem) {
    EXPECT_TRUE(analyze(
        "x = 1\n"
        "f = function(a)\n"
        "  if a then return a end if\n"
        "  return nil\n"
        "end function\n"
        "print(f(x))"
    ));
}

TEST(Semantic, ComplexFunctionSem) {
    EXPECT_TRUE(analyze(
        "makeCounter = function()\n"
        "  count = 0\n"
        "  return function()\n"
        "    count = count + 1\n"
        "    return count\n"
        "  end function\n"
        "end function\n"
        "counter = makeCounter()\n"
        "x = counter()"
    ));
}

TEST(Semantic, RecursiveFunctionSem) {
    EXPECT_TRUE(analyze(
        "factorial = function(n)\n"
        "  if n <= 1 then\n"
        "    return 1\n"
        "  end if\n"
        "  return n * factorial(n - 1)\n"
        "end function\n"
        "x = factorial(5)"
    ));
}

TEST(SemanticError, IndexWithNonNumber) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[\"hello\"]"));
}

TEST(SemanticError, IndexWithBool) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[true]"));
}

TEST(SemanticError, IndexWithFunction) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n f = function() return 1 end function\n x = arr[f]"));
}

TEST(Semantic, IndexWithNumber) {
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n x = arr[0]"));
}

TEST(Semantic, IndexWithVariable) {
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n i = 1\n x = arr[i]"));
}

TEST(Semantic, StringIndexing) {
    EXPECT_TRUE(analyze("s = \"hello\"\n x = s[0]"));
}

TEST(Semantic, SliceArray) {
    EXPECT_TRUE(analyze("arr = [1, 2, 3, 4, 5]\n x = arr[1:3]"));
}

TEST(Semantic, SliceString) {
    EXPECT_TRUE(analyze("s = \"hello world\"\n x = s[0:5]"));
}

TEST(SemanticError, SliceNonSliceable) {
    EXPECT_FALSE(analyze("x = 42\n y = x[0:2]"));
}

TEST(SemanticError, SliceStartNotNumber) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[\"start\":2]"));
}

TEST(SemanticError, SliceEndNotNumber) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[0:\"end\"]"));
}

TEST(Semantic, SliceOnlyStart) {
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n x = arr[1:]"));
}

TEST(SemanticError, AddNumberToFunction) {
    EXPECT_FALSE(analyze("f = function() return 1 end function\n x = 5 + f"));
}

TEST(SemanticError, SubtractStrings) {
    EXPECT_FALSE(analyze("x = \"hello\" - \"world\""));
}

TEST(SemanticError, MultiplyStringByString) {
    EXPECT_FALSE(analyze("x = \"hello\" * \"world\""));
}

TEST(SemanticError, DivideArrayByNumber) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr / 2"));
}

TEST(Semantic, StringConcatenation) {
    EXPECT_TRUE(analyze("x = \"hello\" + \"world\""));
}

TEST(Semantic, StringNumberConcatenation) {
    EXPECT_TRUE(analyze("x = \"count: \" + 42"));
}

TEST(Semantic, ArithmeticOperations) {
    EXPECT_TRUE(analyze("x = 10 - 5 * 2 / 2 + 1"));
}

TEST(Semantic, ComparisonOperations) {
    EXPECT_TRUE(analyze("x = 5 > 3\n y = \"a\" == \"b\"\n z = true != false"));
}

TEST(Semantic, LogicalOperations) {
    EXPECT_TRUE(analyze("x = true and false\n y = true or false"));
}

TEST(SemanticError, NegateString) {
    EXPECT_FALSE(analyze("x = -\"hello\""));
}

TEST(SemanticError, NegateArray) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = -arr"));
}

TEST(SemanticError, NegateFunction) {
    EXPECT_FALSE(analyze("f = function() return 1 end function\n x = -f"));
}

TEST(Semantic, NegateNumber) {
    EXPECT_TRUE(analyze("x = -42"));
}

TEST(Semantic, NegateVariable) {
    EXPECT_TRUE(analyze("y = 10\n x = -y"));
}

TEST(Semantic, LogicalNot) {
    EXPECT_TRUE(analyze("x = not true\n y = not false\n z = not 0"));
}

TEST(Semantic, LogicalNotAnyType) {
    EXPECT_TRUE(analyze("x = not \"hello\"\n y = not [1, 2, 3]"));
}

TEST(SemanticError, CallNonFunction) {
    EXPECT_FALSE(analyze("x = 42\n y = x()"));
}

TEST(SemanticError, CallString) {
    EXPECT_FALSE(analyze("s = \"hello\"\n x = s()"));
}

TEST(SemanticError, CallArray) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr()"));
}

TEST(SemanticError, TooFewArguments) {
    EXPECT_FALSE(analyze("x = abs()"));
}

TEST(SemanticError, TooManyArguments) {
    EXPECT_FALSE(analyze("x = abs(1, 2, 3)"));
}

TEST(SemanticError, WrongArgumentTypeAbs) {
    EXPECT_FALSE(analyze("x = abs(\"hello\")"));
}

TEST(SemanticError, WrongArgumentTypeCeil) {
    EXPECT_FALSE(analyze("x = ceil([1, 2, 3])"));
}

TEST(SemanticError, WrongArgumentTypeSqrt) {
    EXPECT_FALSE(analyze("x = sqrt(true)"));
}

TEST(Semantic, BuiltinWithCorrectArgs) {
    EXPECT_TRUE(analyze("x = abs(-5)\n y = ceil(3.14)\n z = floor(2.7)"));
}

TEST(Semantic, FunctionCall) {
    EXPECT_TRUE(analyze("f = function(x) return x * 2 end function\n y = f(5)"));
}

TEST(Semantic, NestedFunctionCalls) {
    EXPECT_TRUE(analyze("x = abs(ceil(-3.7))"));
}

TEST(SemanticError, SplitWrongFirstArg) {
    EXPECT_FALSE(analyze("x = split(123, \" \")"));
}

TEST(SemanticError, SplitWrongSecondArg) {
    EXPECT_FALSE(analyze("x = split(\"hello world\", 123)"));
}

TEST(SemanticError, JoinWrongFirstArg) {
    EXPECT_FALSE(analyze("x = join(\"hello\", \",\")"));
}

TEST(SemanticError, JoinWrongSecondArg) {
    EXPECT_FALSE(analyze("arr = [\"a\", \"b\"]\n x = join(arr, 123)"));
}

TEST(Semantic, StringBuiltins) {
    EXPECT_TRUE(analyze(
        "s = \"Hello World\"\n"
        "lower_s = lower(s)\n"
        "upper_s = upper(s)\n"
        "parts = split(s, \" \")\n"
        "joined = join(parts, \"-\")\n"
        "replaced = replace(s, \"World\", \"Universe\")"
    ));
}

TEST(SemanticError, VariableOutOfScope) {
    EXPECT_FALSE(analyze(
        "if true then\n"
        "  x = 1\n"
        "end if\n"
        "print(x)"
    ));
}

TEST(SemanticError, VariableInNestedScope) {
    EXPECT_FALSE(analyze(
        "if true then\n"
        "  if true then\n"
        "    y = 2\n"
        "  end if\n"
        "  print(y)\n"
        "end if"
    ));
}

TEST(Semantic, VariableShadowing) {
    EXPECT_TRUE(analyze(
        "x = 1\n"
        "if true then\n"
        "  x = 2\n"
        "  print(x)\n"
        "end if\n"
        "print(x)"
    ));
}

TEST(Semantic, ForLoopVariable) {
    EXPECT_TRUE(analyze(
        "arr = [1, 2, 3]\n"
        "for item in arr\n"
        "  print(item)\n"
        "end for"
    ));
}

TEST(SemanticError, ForLoopVariableOutOfScope) {
    EXPECT_FALSE(analyze(
        "arr = [1, 2, 3]\n"
        "for item in arr\n"
        "  x = item\n"
        "end for\n"
        "print(item)"
    ));
}

TEST(Semantic, ComplexExpressions) {
    EXPECT_TRUE(analyze(
        "arr = [1, 2, 3, 4, 5]\n"
        "sum = 0\n"
        "for x in arr\n"
        "  if x > 2 then\n"
        "    sum = sum + x\n"
        "  end if\n"
        "end for\n"
        "result = sum * 2"
    ));
}

TEST(Semantic, ArrayIndexingInLoop) {
    EXPECT_TRUE(analyze(
        "arr = [10, 20, 30]\n"
        "indices = [0, 1, 2]\n"
        "for i in indices\n"
        "  print(arr[i])\n"
        "end for"
    ));
}

TEST(Semantic, NestedFunctionAccess) {
    EXPECT_TRUE(analyze(
        "outer = function(x)\n"
        "  inner = function(y)\n"
        "    return x + y\n"
        "  end function\n"
        "  return inner\n"
        "end function\n"
        "add5 = outer(5)\n"
        "result = add5(3)"
    ));
}

TEST(Semantic, MultipleFunctionReturns) {
    EXPECT_TRUE(analyze(
        "conditional = function(flag)\n"
        "  if flag then\n"
        "    return function(x) return x * 2 end function\n"
        "  else\n"
        "    return function(x) return x + 1 end function\n"
        "  end if\n"
        "end function\n"
        "doubler = conditional(true)\n"
        "result = doubler(5)"
    ));
}

TEST(Semantic, ArrayManipulation) {
    EXPECT_TRUE(analyze(
        "arr = []\n"
        "push(arr, 1)\n"
        "push(arr, 2)\n"
        "push(arr, 3)\n"
        "insert(arr, 1, 99)\n"
        "last = pop(arr)\n"
        "remove(arr, 0)\n"
        "sort(arr)"
    ));
}

TEST(SemanticError, WrongArrayOperationArgs) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n push(arr, \"too\", \"many\", \"args\")"));
}

TEST(SemanticError, InsertWrongIndexType) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n insert(arr, \"not_number\", 5)"));
}

TEST(SemanticError, RemoveWrongIndexType) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n remove(arr, true)"));
}

TEST(Semantic, StringOperationsComplex) {
    EXPECT_TRUE(analyze(
        "text = \"The Quick Brown Fox\"\n"
        "lower_text = lower(text)\n"
        "words = split(lower_text, \" \")\n"
        "first_word = words[0]\n"
        "length = len(first_word)\n"
        "result = replace(text, \"Fox\", \"Dog\")"
    ));
}

TEST(Semantic, TypeConversions) {
    EXPECT_TRUE(analyze(
        "num = 42\n"
        "str = to_string(num)\n"
        "parsed = parse_num(\"123\")\n"
        "length = len(str)"
    ));
}

TEST(SemanticError, ParseNumWrongType) {
    EXPECT_FALSE(analyze("x = parse_num(123)"));
}

TEST(SemanticError, ToStringTooManyArgs) {
    EXPECT_FALSE(analyze("x = to_string(1, 2)"));
}

TEST(SemanticIndexing, ValidIndexing) {
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n x = arr[0]"));
    EXPECT_TRUE(analyze("s = \"hello\"\n c = s[1]"));
    EXPECT_TRUE(analyze("arr = [1, 2]\n i = 0\n x = arr[i]"));
}

TEST(SemanticIndexingError, NonNumberIndex) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[\"hello\"]"));
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[true]"));
    EXPECT_FALSE(analyze("s = \"test\"\n x = s[[1, 2]]"));
}

TEST(SemanticIndexingError, IndexingNonIndexable) {
    EXPECT_FALSE(analyze("x = 42\n y = x[0]"));
    EXPECT_FALSE(analyze("f = function() return 1 end function\n x = f[0]"));
}

TEST(SemanticSlicingError, NonSliceable) {
    EXPECT_FALSE(analyze("x = 42\n y = x[1:3]"));
    EXPECT_FALSE(analyze("x = true\n y = x[0:1]"));
}

TEST(SemanticSlicingError, NonNumberSliceIndices) {
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[\"start\":2]"));
    EXPECT_FALSE(analyze("arr = [1, 2, 3]\n x = arr[0:\"end\"]"));
    EXPECT_FALSE(analyze("s = \"hello\"\n x = s[true:false]"));
}

TEST(SemanticBinaryOp, ArithmeticErrors) {
    EXPECT_FALSE(analyze("x = \"hello\" - \"world\""));
    EXPECT_FALSE(analyze("x = [1, 2] * 3"));
    EXPECT_FALSE(analyze("x = true / false"));
    EXPECT_FALSE(analyze("f = function() return 1 end function\n x = 5 + f"));
}

TEST(SemanticBinaryOp, StringConcatenation) {
    EXPECT_TRUE(analyze("x = \"hello\" + \"world\""));
    EXPECT_TRUE(analyze("x = \"number: \" + 42"));
    EXPECT_TRUE(analyze("x = 42 + \" is the answer\""));
}

TEST(SemanticBinaryOp, ValidComparisons) {
    EXPECT_TRUE(analyze("x = 5 > 3"));
    EXPECT_TRUE(analyze("x = \"a\" == \"b\""));
    EXPECT_TRUE(analyze("x = true != false"));
    EXPECT_TRUE(analyze("x = [1, 2] == [1, 2]"));
}

TEST(SemanticUnaryOp, ValidUnaryOps) {
    EXPECT_TRUE(analyze("x = -42"));
    EXPECT_TRUE(analyze("y = 10\n x = -y"));
    EXPECT_TRUE(analyze("x = not true"));
    EXPECT_TRUE(analyze("x = not 0"));
    EXPECT_TRUE(analyze("x = not \"hello\""));
}

TEST(SemanticUnaryOpError, InvalidNegation) {
    EXPECT_FALSE(analyze("x = -\"hello\""));
    EXPECT_FALSE(analyze("arr = [1, 2]\n x = -arr"));
    EXPECT_FALSE(analyze("f = function() return 1 end function\n x = -f"));
}

TEST(SemanticRecursion, DirectRecursion) {
    EXPECT_TRUE(analyze(
        "fibonacci = function(n)\n"
        "  if n <= 1 then return n end if\n"
        "  return fibonacci(n-1) + fibonacci(n-2)\n"
        "end function\n"
        "result = fibonacci(10)"
    ));
}

TEST(SemanticTyping, MixedExpressions) {
    EXPECT_TRUE(analyze(
        "num = 42\n"
        "str = \"number: \"\n"
        "combined = str + num\n"
        "length = len(combined)"
    ));
}

TEST(SemanticTypingError, IncompatibleOperations) {
    EXPECT_FALSE(analyze("result = [1, 2, 3] - 5"));
    EXPECT_FALSE(analyze("result = true * false"));
}

TEST(SemanticBuiltins, MathFunctions) {
    EXPECT_TRUE(analyze("x = abs(-5) + ceil(3.2) + floor(7.8) + round(4.5)"));
    EXPECT_TRUE(analyze("y = sqrt(16) + rnd()"));
}

TEST(SemanticBuiltins, StringFunctions) {
    EXPECT_TRUE(analyze(
        "text = \"Hello World\"\n"
        "low = lower(text)\n"
        "up = upper(text)\n"
        "parts = split(text, \" \")\n"
        "joined = join(parts, \"-\")\n"
        "replaced = replace(text, \"World\", \"Universe\")"
    ));
}

TEST(SemanticIntegration, ComplexDataProcessing) {
    EXPECT_TRUE(analyze(
        "data = [\"apple\", \"banana\", \"cherry\"]\n"
        "process = function(items)\n"
        "  result = []\n"
        "  for item in items\n"
        "    processed = upper(item)\n"
        "    if len(processed) > 5 then\n"
        "      push(result, processed)\n"
        "    end if\n"
        "  end for\n"
        "  return result\n"
        "end function\n"
        "filtered = process(data)\n"
        "output = join(filtered, \", \")"
    ));
}

TEST(SemanticIntegration, CalculatorExample) {
    EXPECT_TRUE(analyze(
        "calculator = function(op, a, b)\n"
        "  if op == \"+\" then return a + b end if\n"
        "  if op == \"-\" then return a - b end if\n"
        "  if op == \"*\" then return a * b end if\n"
        "  if op == \"/\" then return a / b end if\n"
        "  return nil\n"
        "end function\n"
        "result1 = calculator(\"+\", 5, 3)\n"
        "result2 = calculator(\"*\", result1, 2)"
    ));
}

TEST(Semantic, SliceOnlyEnd) {
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n x = arr[:2]"));
}

TEST(SemanticSlicing, ValidSlicing) {
    EXPECT_TRUE(analyze("arr = [1, 2, 3, 4]\n x = arr[1:3]"));
    EXPECT_TRUE(analyze("s = \"hello\"\n sub = s[0:2]"));
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n x = arr[:]"));
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n x = arr[1:]"));
    EXPECT_TRUE(analyze("arr = [1, 2, 3]\n x = arr[:2]"));
}
