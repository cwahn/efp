add_executable(demo demo.cpp)
target_link_libraries(demo
    PRIVATE
    efp)

# add_executable(format_demo format_demo.cpp)
# target_link_libraries(format_demo
# PRIVATE
# efp)
add_executable(io_demo io_demo.cpp)
target_link_libraries(io_demo
    PRIVATE
    efp)

# add_executable(loop_c loop_c.cpp)
# target_link_libraries(loop_c
# PRIVATE
# efp)

# add_executable(loop_efp loop_efp.cpp)
# target_link_libraries(loop_efp
# PRIVATE
# efp)
add_executable(trait_demo trait_demo.cpp)
target_link_libraries(trait_demo
    PRIVATE
    efp)

add_executable(all_test test.cpp)
target_link_libraries(all_test
    PRIVATE
    Catch2::Catch2WithMain
    efp)

catch_discover_tests(all_test)
