cmake_minimum_required(VERSION 3.15)
project(ex1 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex1 manageStudents.c)
target_compile_options(ex1 PRIVATE -Wall -Wextra -Wvla -Werror)
add_executable(ex1_ tester.c)