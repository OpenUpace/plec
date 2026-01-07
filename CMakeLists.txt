cmake_minimum_required(VERSION 3.10...3.28) # Just Ai-Upace used
project(catmc)

set(CMAKE_CXX_STANDARD 11)

add_executable(catmc catmc.cpp)

add_compile_options(-Wall -Wextra -Werror)
