cmake_minimum_required(VERSION 3.14)

add_library(syntax STATIC
        syntax.h
        syntax.cpp
        errors/synt_errors.h
        errors/synt_errors.cpp
)

target_link_libraries(syntax PUBLIC
        lexer
        vls_and_sttmnts
)

target_include_directories(syntax PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
