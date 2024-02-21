include(CTest)

include(FetchContent)

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

add_executable(efp_test test.cpp)
target_link_libraries(efp_test
    PRIVATE
    Catch2::Catch2WithMain
    efp)

catch_discover_tests(efp_test)
enable_template_compilation_time(efp_test)