cmake_minimum_required(VERSION 3.23)

project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        lab1
        src/main.cpp
        src/lib/command/ConverterCommand.h
        src/lib/converter/ConversionType.h
        src/lib/csv/CsvReader.h
        src/lib/converter/MealyToMoorConverter.h
        src/lib/statemachine/MealyStateMachineBuilder.h
        src/lib/statemachine/MealyStateMachine.h
        src/lib/statemachine/MoorStateMachine.h
        src/lib/statemachine/MoorStateMachineBuilder.h
        src/lib/csv/CsvWriter.h
        src/lib/converter/MoorToMealyConverter.h
)
