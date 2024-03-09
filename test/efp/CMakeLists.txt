include(FetchContent)

find_package(Catch2 QUIET)

if(NOT Catch2_FOUND)
message("Catch2 not found on the system.\nBuild from source.")
FetchContent_Declare(
    Catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.0.tar.gz
    )
    FetchContent_MakeAvailable(Catch2)
endif()
    
include(CTest)
include(Catch)
enable_testing()

# Function to configure and create a test target for a specific C++ standard
function(create_test_for_standard cpp_standard)
    set(target_name "efp_test_cpp${cpp_standard}")
    add_executable(${target_name} test.cpp)
    target_compile_features(${target_name} PRIVATE cxx_std_${cpp_standard})
    set_target_properties(${target_name} PROPERTIES CXX_EXTENSIONS OFF)
    target_link_libraries(${target_name}
        PRIVATE
        Catch2::Catch2WithMain
        efp)
    catch_discover_tests(${target_name})
    # Enable compilation time output
    efp_enable_compilation_time(${target_name})

    target_compile_options(${target_name} PRIVATE
  -ftemplate-backtrace-limit=0
  -fdiagnostics-show-template-tree
)
endfunction()

# Create tests for C++11 through C++23
create_test_for_standard("11")
create_test_for_standard("14")
create_test_for_standard("17")
create_test_for_standard("20")
create_test_for_standard("23")

# Define a custom target to run all tests
add_custom_target(efp_run_all_tests
    COMMENT "Running all test targets for each C++ standard version..."
)

# Iterate over all test targets and add them as dependencies to run_all_tests
foreach(test_target IN LISTS all_test_targets)
    add_custom_command(TARGET run_all_tests
                       POST_BUILD
                       COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "^${test_target}$"
                       COMMENT "Running tests for target: ${test_target}")
endforeach()