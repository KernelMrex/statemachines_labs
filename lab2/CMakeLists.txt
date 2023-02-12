cmake_minimum_required(VERSION 3.23)

project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        lab2
        src/main.cpp
        src/lib/args/Args.h
        src/lib/statemachine/StateMachineType.h
        src/lib/csv/CSVWriter.h
        src/lib/csv/CSVReader.h
        src/lib/csv/Types.h
        src/lib/statemachine/Minimization.h
        src/lib/statemachine/Minimization.cpp
        src/lib/utils/vector.h
)
