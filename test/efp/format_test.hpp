#ifndef FORMAT_TEST_HPP_
#define FORMAT_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp/format.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("format", "[format]") {
    CHECK(format("{}", 42) == "42");
    CHECK(format("{}", 42.0) == "42.000000");
    CHECK(format("{}", "hello") == "hello");
    CHECK(format("{}", 'c') == "c");
    CHECK(format("{}", 42u) == "42");
    CHECK(format("{}", 42l) == "42");
    CHECK(format("{}", 42ll) == "42");
    CHECK(format("{}", 42.0f) == "42.000000");
    CHECK(format("{}", 42.0l) == "42.000000");

    // format specifiers
    // CHECK(format("{:d}", 42) == "42");
    // CHECK(format("{:f}", 42.0) == "42.000000");
    // CHECK(format("{:s}", "hello") == "hello");
    // CHECK(format("{:c}", 'c') == "c");
    // // CHECK(format("{:u}", 42u) == "42");
    // // CHECK(format("{:l}", 42l) == "42");
    // // CHECK(format("{:ll}", 42ll) == "42");
    // CHECK(format("{:f}", 42.0f) == "42.000000");
    // CHECK(format("{:f}", 42.0l) == "42.000000");
}

#endif