cmake_minimum_required(VERSION 3.15)
project(ex2 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex3 RBTree.c)
target_compile_options(ex3 PRIVATE -Wall -Wextra -Wvla -Werror)
#add_executable(ex2_ tester.c)