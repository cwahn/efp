cmake_minimum_required(VERSION 3.11)

project(
    efp
    VERSION 0.1
    DESCRIPTION "Eager functional programming library for C++ 11"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED On)
# set(CMAKE_CXX_COMPILER /usr/bin/clang++)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)

find_package(Catch2 3 REQUIRED)

include(CTest)
include(Catch)
enable_testing()

add_library(efp INTERFACE)
target_include_directories(efp INTERFACE "./include")

add_subdirectory("./test")