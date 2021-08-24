cmake_minimum_required(VERSION 3.12)
project(AVL C)

set(CMAKE_C_STANDARD 99)

add_executable(AVL avl.h avl.c main.c)