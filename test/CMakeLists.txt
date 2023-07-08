add_executable(prelude_test prelude_test.cpp)
target_link_libraries(prelude_test
    PRIVATE
    Catch2::Catch2WithMain
    efp)

catch_discover_tests(prelude_test)

add_custom_command(
    TARGET prelude_test
    COMMENT "Run tests"
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> "prelude_test"  --output-on-failure
)