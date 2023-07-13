add_executable(loop_c loop_c.cpp)
target_link_libraries(loop_c
    PRIVATE
    efp)

add_executable(loop_efp loop_efp.cpp)
target_link_libraries(loop_efp
    PRIVATE
    efp)

add_executable(all_test test.cpp)

target_link_libraries(all_test
    PRIVATE
    Catch2::Catch2WithMain
    efp)

catch_discover_tests(all_test)

add_custom_command(
    TARGET all_test
    COMMENT "Run tests"
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> "all_test" --output-on-failure
)
