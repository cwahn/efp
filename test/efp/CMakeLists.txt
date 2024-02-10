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

add_executable(all_test test.cpp)
target_link_libraries(all_test
    PRIVATE
    Catch2::Catch2WithMain
    efp)

catch_discover_tests(all_test)