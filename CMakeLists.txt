cmake_minimum_required(VERSION 3.15)
project(Differentiator)

set(CMAKE_CXX_STANDARD 14)

include_directories(/Users/alex/Desktop/my_libs)

add_executable(Differentiator main.cpp Diff_Tree.h)