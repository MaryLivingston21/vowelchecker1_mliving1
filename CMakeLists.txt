cmake_minimum_required(VERSION 3.13)
project(vowelchecker1_mliving1 C)

set(CMAKE_C_STANDARD 99)

add_executable(vowelchecker1_mliving1 pthreads-example.c vowelchecker1.mliving1.c vowelchecker2.mliving1.c mutex-example-threads.c)