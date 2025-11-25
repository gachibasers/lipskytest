cmake_minimum_required(VERSION 3.16)
project(RPG_Simulation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)

set(GTEST_DONT_DEFINE_TEST_MACRO_BY_DEFAULT ON)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

enable_testing()

add_executable(game_app
        main.cpp
        Strategy.hpp
        Item.hpp
        Character.hpp
        Analyzer.hpp
)
add_subdirectory(tests)