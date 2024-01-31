add_executable(all_test test.cpp)
target_link_libraries(all_test
    PRIVATE
    Catch2::Catch2WithMain
    efp)

catch_discover_tests(all_test)