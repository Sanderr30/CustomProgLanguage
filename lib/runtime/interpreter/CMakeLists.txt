cmake_minimum_required(VERSION 3.14)

add_library(interpreter STATIC
    interpreter.h
    interpreter.cpp

    statements/statements.h
    statements/statements.cpp

    builtins/builtins.h
    builtins/builtins.cpp

    errors/intrptr_errors.cpp
    errors/intrptr_errors.h

    builtins/errors/bltns_errors.cpp
    builtins/errors/bltns_errors.h
)

target_link_libraries(interpreter PUBLIC
    value
    function
    enviroment
    evaluator
    syntax
    semantic
    vls_and_sttmnts
)

target_include_directories(interpreter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
