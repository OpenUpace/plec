cmake_minimum_required(VERSION 3.10...3.28) # Just Ai-Upace used
project(musec)

set(CMAKE_CXX_STANDARD 11)

add_executable(musec musec.cpp)

add_compile_options(-Wall -Wextra -Werror)
