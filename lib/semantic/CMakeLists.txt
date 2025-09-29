cmake_minimum_required(VERSION 3.14)

add_library(semantic STATIC
        semantic.h
        semantic.cpp

        symbols/symb_table.h
        symbols/symb_table.cpp

        errors/semantic_errors.h

        type_system/type_system.h
        type_system/type_system.cpp
)

target_link_libraries(semantic PUBLIC
        syntax
)

target_include_directories(semantic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)