cmake_minimum_required(VERSION 3.23)

project(lab3-task1)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        lab3-task1
        src/main.cpp
        src/lib/args/Args.h
        src/lib/grammar/GrammarType.h
)
