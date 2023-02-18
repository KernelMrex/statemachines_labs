cmake_minimum_required(VERSION 3.23)

project(lab3-task1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(
        lab3-task1
        src/main.cpp
        src/lib/args/Args.h
        src/lib/grammar/GrammarType.h
        src/lib/grammar/Grammar.h
        src/lib/grammar/Grammar.cpp
        src/lib/graph/CGraph.h
)

add_executable(
        lab3-task1_unit
        src/lib/args/Args.h
        src/lib/grammar/GrammarType.h
        src/lib/grammar/Grammar.h
        src/lib/grammar/Grammar.cpp
        src/lib/graph/CGraph.h

        test/unit/graph/CGraphTest.cpp
        test/unit/grammar/GrammarTest.cpp
)


target_link_libraries(lab3-task1_unit gtest gtest_main gmock)