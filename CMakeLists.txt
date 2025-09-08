cmake_minimum_required(VERSION 3.28.3)
project(vlc)

set(CMAKE_CXX_STANDARD 11)

add_executable(vlc
        src/lexer.cpp
        src/main.cpp
        include/parser.h
        src/parser.cpp
        include/ast.h
        src/ast.cpp)

target_include_directories(vlc PRIVATE include)

add_compile_options(-g)