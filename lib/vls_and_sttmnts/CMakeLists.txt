cmake_minimum_required(VERSION 3.14)

add_library(vls_and_sttmnts STATIC
        vls_and_sttmnts.h
        vls_and_sttmnts.cpp
)

target_link_libraries(vls_and_sttmnts PUBLIC
        lexer
)

target_include_directories(vls_and_sttmnts PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
