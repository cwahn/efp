cmake_minimum_required(VERSION 3.11)

project(
    efp
    VERSION 0.1
    DESCRIPTION "Eager functional programming library for C++ 11"
    LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    message(WARNING "Copy-elision (RVO, NRVO) is not guraanteed.\n") 
    if(MSVC_VERSION VERSION_LESS "1934")
        message(WARNING  "NVRO is not encouraged. Zc:NRVO not supported under MSVC 17.4.")
    endif()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED On)
# set(CMAKE_CXX_COMPILER /usr/bin/clang++)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)

if(MSVC)
    if(MSVC_VERSION VERSION_GREATER_EQUAL "1934")
        string(APPEND CMAKE_CXX_FLAGS " /Zc:nrvo")
    endif()
endif()

find_package(Catch2 3 REQUIRED)

include(CTest)
include(Catch)
enable_testing()

add_library(efp INTERFACE)
target_include_directories(efp INTERFACE "./include")

add_subdirectory("./test")