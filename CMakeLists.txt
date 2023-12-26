cmake_minimum_required(VERSION 3.5.0)

# CMake 3.24 and above prefers to set the timestamps of all extracted contents to the time of the extraction.
# This ensures that anything that depends on the extracted contents will be rebuilt whenever the URL changes.
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

project(
    efp
    VERSION 0.1
    DESCRIPTION "Eager functional programming library for C++ 11"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    message(WARNING "Copy-elision (RVO, NRVO) is not guraanteed.\n")

    if(MSVC_VERSION VERSION_LESS "1934")
        message(WARNING "NVRO is not encouraged. Zc:N  RVO not supported under MSVC 17.4.")
    endif()
endif()

if(MSVC)
    if(MSVC_VERSION VERSION_GREATER_EQUAL "1934")
        string(APPEND CMAKE_CXX_FLAGS " /Zc:nrvo")
    endif()
endif()

# Check for pthread_create function
include(CheckFunctionExists)
check_function_exists(pthread_create HAVE_PTHREAD)

# If pthread_create is found, define a preprocessor macro
if(HAVE_PTHREAD)
    add_definitions(-DHAVE_POSIX)
endif()


include(CTest)

find_package(Catch2 QUIET)

if(NOT Catch2_FOUND)
    # message("Catch2 not found on the system.\nBuild from source.")
    FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.tar.gz
        URL_HASH MD5=2c802a4938ed842e2942c60d1d231bb7
    )
    FetchContent_MakeAvailable(Catch2)
endif()

include(Catch)
enable_testing()

add_library(efp INTERFACE)
target_include_directories(efp INTERFACE "./include")

add_subdirectory("./test")