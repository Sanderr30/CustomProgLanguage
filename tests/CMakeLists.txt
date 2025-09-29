include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  itmoscript_tests
  lexer_tests.cpp
  syntax_tests.cpp
  semantic_tests.cpp
  function_test.cpp
  types_test.cpp
  loop_and_branch_test.cpp
  runtime_tests.cpp
  operations_tests.cpp
  integration_tests.cpp
  builtns_tests.cpp
  func_tests.cpp
)

target_link_libraries(
    itmoscript_tests
    interpreter
    lexer
    semantic
    syntax
    GTest::gtest_main
)

target_include_directories(itmoscript_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(itmoscript_tests)
