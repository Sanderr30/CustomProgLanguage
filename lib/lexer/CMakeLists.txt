cmake_minimum_required(VERSION 3.14)

add_library(lexer STATIC
        rules/rules.h
        token/token.h
        token/token.cpp
        errors/lex_errors.cpp
        errors/lex_errors.h
        lexer.h
        lexer.cpp
)

target_include_directories(lexer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)