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

# AddressSanitizer
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if(ENABLE_ASAN)
    # Add the -fsanitize=address flag to the compiler and linker
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

# Function to enable template compilation time measurement for a specific target
function(efp_enable_compilation_time target_name)
    message(STATUS "Enabling template compilation time measurement for target: ${target_name}")

    # Check if the compiler is Clang or GCC and add the respective flags to the target
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Clang-specific flags to report template instantiation times for the target
        set_property(TARGET ${target_name} APPEND_STRING PROPERTY COMPILE_FLAGS " -ftime-trace")
        message(STATUS "Added Clang-specific flags for template instantiation times to ${target_name}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # GCC-specific flags to report template instantiation times for the target
        set_property(TARGET ${target_name} APPEND_STRING PROPERTY COMPILE_FLAGS " -ftemplate-depth=1024 -ftime-report")
        message(STATUS "Added GCC-specific flags for template instantiation times to ${target_name}")
    endif()
endfunction()

# ! temp
# Set verbose makefile output
# set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Enable verbose output" FORCE)

add_library(efp INTERFACE)
target_include_directories(efp INTERFACE "./include")

# Check if this CMakeLists.txt is the top-most CMake project
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory("./example")
    add_subdirectory("./test")
endif()