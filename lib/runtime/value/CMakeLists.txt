cmake_minimum_required(VERSION 3.14)

add_library(value STATIC
    value.cpp
    value.h
    errors/val_errors.h
    errors/val_errors.cpp
)

target_link_libraries(value PUBLIC
        semantic
)

target_include_directories(value PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
