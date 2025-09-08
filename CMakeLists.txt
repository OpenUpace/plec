cmake_minimum_required(VERSION 3.28.3)
project(vlc)

set(CMAKE_CXX_STANDARD 11)

add_executable(vlc
        src/Lexer.cpp
        src/main.cpp
        include/Parser.h
        src/Parser.cpp
        include/ast.h)

target_include_directories(vlc PRIVATE include)
