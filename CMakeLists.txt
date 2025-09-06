cmake_minimum_required(VERSION 3.28.3)
project(vlc)

set(CMAKE_CXX_STANDARD 11)

add_executable(vlc
        src/Lexer.cpp
        src/Parser.cpp
        src/AST.cpp
        src/main.cpp)

target_include_directories(vlc PRIVATE include)
