cmake_minimum_required(VERSION 3.14)

add_library(enviroment STATIC
    enviroment.h
    enviroment.cpp
    errors/env_errors.h
    errors/env_errors.cpp
)

target_link_libraries(enviroment PUBLIC
        value
)

target_include_directories(enviroment PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
