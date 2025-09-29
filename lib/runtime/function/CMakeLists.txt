cmake_minimum_required(VERSION 3.14)

add_library(function STATIC
    function.h
    function.cpp
    errors/func_errors.h
    errors/func_errors.cpp
)

target_link_libraries(function PUBLIC
    value
    vls_and_sttmnts
)

target_include_directories(function PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
