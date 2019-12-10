cmake_minimum_required(VERSION 3.13.4)
project(ex2 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex2 TreeAnalyzer.c queue.c)
target_compile_options(ex2 PRIVATE -Wall -Wextra -Wvla -Werror)
