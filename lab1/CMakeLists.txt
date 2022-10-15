cmake_minimum_required(VERSION 3.23)

project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        lab1
        src/main.cpp
        src/lib/ConverterCommand.h
        src/lib/ConversionType.h
        src/lib/CsvReader.h
        src/lib/StateMachineConverter.h
        src/lib/ConvertingStrategy.h
        src/lib/MealyToMoorConvertingStrategy.h
        src/lib/MealyStateMachineBuilder.h
        src/lib/MealyStateMachine.h
)
