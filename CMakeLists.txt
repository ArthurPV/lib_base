project(lib_base)
cmake_minimum_required(VERSION 3.11)

add_subdirectory(tests)

add_library(lib_base src/lib_base.c)
