cmake_minimum_required(VERSION 3.14)

add_library(evaluator STATIC
    evaluator.h
    evaluator.cpp

    operations/register.h
    operations/register.cpp

    operations/handlers.h
    operations/handlers.cpp

    errors/ev_errors.cpp
    errors/ev_errors.h
)

target_link_libraries(evaluator PUBLIC
    value
    function
    enviroment
    vls_and_sttmnts
    semantic
)

target_include_directories(evaluator PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
