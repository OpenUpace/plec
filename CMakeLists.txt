cmake_minimum_required(VERSION 3.10...3.28) # Just Ai-Upace used
project(catmc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(catmc src/Lex/Lexer.cpp
                     src/Parser/Parser.cpp)

