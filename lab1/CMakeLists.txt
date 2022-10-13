cmake_minimum_required(VERSION 3.23)

project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        lab2
        src/main.cpp
        src/lib/ConverterCommand.h
        src/lib/ConversionType.h
)
